#include "game_operator.h"

#include <cstdint>
#include <random>
#include <utility>

#include "common/shared_constants.h"

#include "ticks.h"

const int16_t NEAR_CELLS = 3;
const int16_t COLLECTABLE_SPAWN_IT = TICKS * 15;
const int16_t COLLECTABLE_EXTRA_SPAWN_TIME = TICKS * 5;


GameOperator::GameOperator(): collisions(), collectables(collisions, players) {}

void GameOperator::load_map(MapDto& map_dto) {
    collisions.load_map(map_dto);
    spawns.clear();
    Spawn spawn = {200, 200, 0, 50, true, 0};
    spawns.push_back(spawn);
    spawn = {250, 200, 1, 0, true, 0};
    spawns.push_back(spawn);
    spawn = {300, 200, 1, 0, true, 0};
    spawns.push_back(spawn);
    spawn = {350, 200, 1, 0, true, 0};
    spawns.push_back(spawn);
}

void GameOperator::initialize_players(const std::vector<uint8_t>& duck_ids) {
    std::vector<std::pair<int16_t, int16_t>> spawn_points = {
            {50, 25}, {MAP_WIDTH_PIXELS - 50, 50}, {50, 25}, {MAP_WIDTH_PIXELS - 50, 200}};
    players.clear();

    for (uint8_t i = 0; i < duck_ids.size(); ++i) {
        DuckPlayer player(collectables, collisions);
        player.set_coordenades_and_id(spawn_points[i].first, spawn_points[i].second, duck_ids[i]);
        players.emplace(duck_ids[i], std::move(player));
    }
}

void GameOperator::delete_duck_player(uint8_t id_duck) { players.erase(id_duck); }

void GameOperator::initialize_game(MapDto& map_dto, const std::vector<uint8_t>& duck_ids) {
    load_map(map_dto);
    initialize_players(duck_ids);
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
