#include "game_operator.h"

#include <cstdint>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include <sys/types.h>

#include "common/commands.h"
#include "common/config.h"
#include "common/map.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"
#include "server/game/boxes.h"
#include "server/game/collisions.h"

static Config& config = Config::get_instance();

const int16_t NEAR_CELLS = 3;
const static int TICKS = config.get_server_ticks();
const static int16_t COLLECTABLE_SPAWN_IT = TICKS * 15;
const static int16_t COLLECTABLE_EXTRA_SPAWN_TIME = TICKS * 5;
const static bool CHEATS = config.get_cheats_on();


GameOperator::GameOperator(): collisions(), collectables(collisions, players, boxes) {}

void GameOperator::load_map(const Map& map_info) {
    collisions.load_map(map_info.map_dto);
    spawns.clear();
    for (const auto& coords: map_info.collectables_spawns) {
        Spawn act_spawn{static_cast<int16_t>(coords.first * BLOCK_SIZE),
                        static_cast<int16_t>(coords.second * BLOCK_SIZE),
                        0,
                        0,
                        true,
                        0};
        spawns.push_back(act_spawn);
    }
}

void GameOperator::initialize_players(
        const std::vector<std::pair<uint8_t, std::string>>& ducks_info, const Map& map_info,
        bool first_round) {
    const std::vector<std::pair<int16_t, int16_t>>& spawn_points = map_info.duck_spawns;
    players.clear();
    for (auto& [duck_id, name]: ducks_info) {
        add_player(spawn_points, duck_id, name, first_round);
    }
}

void GameOperator::add_player(const std::vector<std::pair<int16_t, int16_t>>& spawn_points,
                              uint8_t duck_id, const std::basic_string<char>& name,
                              bool first_round) {
    DuckPlayer player(collectables, collisions, spawn_points[duck_id].first * BLOCK_SIZE,
                      spawn_points[duck_id].second * BLOCK_SIZE, duck_id, name);
    players.emplace(duck_id, std::move(player));
    if (first_round)
        players.at(duck_id).cheats_on.infiniteHP = true;
}

void GameOperator::initialize_boxes(const Map& map_info) {
    boxes.clear();
    uint32_t id = 0;
    for (auto const& box: map_info.boxes_spawns) {
        boxes.emplace(id,
                      BoxEntity(box.first * BLOCK_SIZE, box.second * BLOCK_SIZE, id, collisions));
        ++id;
    }
}

void GameOperator::delete_duck_player(uint8_t id_duck) { players.erase(id_duck); }

void GameOperator::initialize_game(const Map& map_info,
                                   const std::vector<std::pair<uint8_t, std::string>>& ducks_info,
                                   bool first_round) {
    load_map(map_info);
    initialize_players(ducks_info, map_info, first_round);
    if (!first_round)
        initialize_boxes(map_info);

    collectables.reset_collectables();
}

bool GameOperator::check_start_game() {
    for (auto& [duck_id, duck]: players) {
        if (boxes.find(duck_id) != boxes.end()) {
            return false;
        }
    }
    boxes.clear();
    return true;
}

void GameOperator::process_action(action& action, bool first_round) {
    if (players.find(action.duck_id) == players.end()) {
        return;
    }
    DuckPlayer& player = players.at(action.duck_id);
    switch (action.command) {
        case StartMovingRight:
            player.run(true);
            break;
        case StartMovingLeft:
            player.run(false);
            break;
        case StopMoving:
            player.stop_running();
            break;
        case StartShooting:
            player.shoot();
            break;
        case StopShooting:
            player.stop_shooting();
            break;
        case LayDown:
            player.lay_down();
            break;
        case StandUp:
            player.stand_up();
            break;
        case StartLookup:
            player.face_up();
            break;
        case StopLookup:
            player.stop_face_up();
            break;
        case Jump:
            player.jump();
            break;
        case StopJump:
            player.stop_jump();
            break;
        case PickUp:
            check_spawn_picked(player.drop_and_pickup());
            break;
        default:
            if (!first_round)
                handle_cheat(player, action.command);
            break;
    }
}

void GameOperator::handle_cheat(DuckPlayer& duck, Command command) {
    if (!CHEATS)
        return;
    if (command == KillEveryone) {
        for (auto& [id, other_duck]: players) {
            if (other_duck.status.duck_id == duck.status.duck_id) {
                continue;
            }
            other_duck.die();
        }
    } else if (command == InfiniteAmmo) {
        if (!duck.equipped_gun)
            return;
        duck.equipped_gun->infinite_ammo();
    } else if (command == GetDeathLaser) {
        Gun new_gun = {0, DeathLaser, static_cast<int16_t>(duck.hitbox.coords.x),
                       static_cast<int16_t>(duck.hitbox.coords.y)};
        duck.drop_collectable();
        duck.equipped_gun = collectables.add_death_laser(new_gun);
    } else if (command == InfiniteHP) {
        duck.infinite_hp();
    } else if (command == FlyMode) {
        duck.fly_mode();
    }
}

void GameOperator::check_spawn_picked(uint32_t id) {
    if (id == 0)
        return;
    for (auto& spawn: spawns) {
        if (spawn.collectable_id != id)
            continue;
        spawn.picked = true;
        spawn.collectable_id = 0;
        spawn.it_since_picked = 0;
        spawn.it_to_spawn = COLLECTABLE_SPAWN_IT + rand() % COLLECTABLE_EXTRA_SPAWN_TIME;
    }
}

void GameOperator::check_broken_boxes() {
    std::vector<uint32_t> id_to_eliminate;
    for (auto& [id, box]: boxes) {
        if (box.destroyed()) {
            GunType n_gun = get_random_guntype(true);
            Coordenades coords = box.get_coords();
            Gun new_gun = {0, n_gun, static_cast<int16_t>(coords.x),
                           static_cast<int16_t>(coords.y)};
            collectables.add_gun(new_gun);
            id_to_eliminate.push_back(id);
        }
    }
    for (uint32_t pos: id_to_eliminate) {
        boxes.erase(pos);
    }
}

void GameOperator::update_game_status() {
    collectables.update_guns_and_bullets();
    for (auto& [id, duck]: players) {
        duck.move_duck();
        duck.update_gun_status();
    }
    verify_spawn();
    check_broken_boxes();
}

GunType GameOperator::get_random_guntype(bool with_exploded_grenade) {
    std::random_device rd;
    std::mt19937 gen(rd());
    int max = with_exploded_grenade ? GunTypeCount : GunTypeCount - 1;
    int min = with_exploded_grenade ? 0 :
                                      1;  // 0 es None, lo interpretamos como una granada explotada
    std::uniform_int_distribution<> dis(min, max);
    return static_cast<GunType>(dis(gen));
}

void GameOperator::verify_spawn() {
    for (auto& spawn: spawns) {
        if (!spawn.picked)
            continue;

        if (spawn.it_since_picked > spawn.it_to_spawn) {
            Gun new_gun = {0, get_random_guntype(false), spawn.x, spawn.y};
            spawn.collectable_id = collectables.add_gun(new_gun);
            spawn.it_since_picked = 0;
            spawn.picked = false;
        } else {
            ++spawn.it_since_picked;
        }
    }
}

void GameOperator::get_snapshot(Snapshot& snapshot) {
    for (auto& [id, duck]: players) {
        snapshot.ducks.push_back(duck.get_status());
    }
    for (auto& [id, box]: boxes) {
        snapshot.boxes.push_back(box.get_info());
    }
    collectables.add_guns_to_snapshot(snapshot);
}

GameOperator::~GameOperator() {}
