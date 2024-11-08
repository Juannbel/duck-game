#include "game_operator.h"

#include <cstdint>
#include <random>
#include <utility>

#include "common/commands.h"
#include "common/map.h"
#include "common/shared_constants.h"

#include "ticks.h"

const int16_t NEAR_CELLS = 3;
const int16_t COLLECTABLE_SPAWN_IT = TICKS * 15;
const int16_t COLLECTABLE_EXTRA_SPAWN_TIME = TICKS * 5;


GameOperator::GameOperator(): collisions(), collectables(collisions, players) {}

void GameOperator::load_map(const Map& map_info) {
    collisions.load_map(map_info.map_dto);
    spawns.clear();
    for (const auto& coords: map_info.collectables_spawns) {
        Spawn act_spawn{static_cast<int16_t>(coords.first * BLOCK_SIZE),
                        static_cast<int16_t>(coords.second * BLOCK_SIZE),
                        0,
                        100,
                        true,
                        0};
        spawns.push_back(act_spawn);
    }
}

void GameOperator::initialize_players(
        const std::vector<std::pair<uint8_t, std::string>>& ducks_info, const Map& map_info) {
    const auto& spawn_points = map_info.duck_spawns;
    players.clear();
    for (auto& duck: ducks_info) {
        DuckPlayer player(collectables, collisions, spawn_points[duck.first].first * BLOCK_SIZE,
                          spawn_points[duck.first].second * BLOCK_SIZE, duck.first, duck.second);
        // player.set_coordenades_and_id(spawn_points[duck.first].first,
        // spawn_points[duck.first].second, duck.first); player.set_player_name(duck.second);
        players.emplace(duck.first, std::move(player));
    }
}

void GameOperator::delete_duck_player(uint8_t id_duck) { players.erase(id_duck); }

void GameOperator::initialize_game(Map& map_info,
                                   const std::vector<std::pair<uint8_t, std::string>>& ducks_info) {
    load_map(map_info);
    initialize_players(ducks_info, map_info);
    collectables.reset_collectables();
}

void GameOperator::process_action(action& action) {
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
            break;
    }
}

void GameOperator::check_spawn_picked(uint32_t id) {
    if (id > 0) {
        for (auto& spawn: spawns) {
            if (spawn.collectable_id == id) {
                spawn.picked = true;
                spawn.collectable_id = 0;
                spawn.it_since_picked = 0;
                spawn.it_to_spawn = COLLECTABLE_SPAWN_IT + rand() % COLLECTABLE_EXTRA_SPAWN_TIME;
            }
        }
    }
}

void GameOperator::update_game_status() {
    for (auto& [id, duck]: players) {
        duck.move_duck();
        duck.update_gun_status();
    }
    // Actualizar la posicion de las balas y vida de los patos si les pegan
    verify_spawn();
    collectables.move_guns_falling();
}

GunType GameOperator::get_random_guntype() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, GunTypeCount - 1);  // 0 es None
    return static_cast<GunType>(dis(gen));
}

void GameOperator::verify_spawn() {
    for (auto& spawn: spawns) {
        if (!spawn.picked)
            continue;

        if (spawn.it_since_picked > spawn.it_to_spawn) {
            uint32_t collectable_id = collectables.get_and_inc_collectable_id();

            Gun new_gun = {collectable_id, get_random_guntype(), spawn.x, spawn.y};
            collectables.add_gun(new_gun);
            spawn.collectable_id = collectable_id;
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
    collectables.add_guns_to_snapshot(snapshot);
}

GameOperator::~GameOperator() {}
