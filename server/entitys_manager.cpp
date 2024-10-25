#include "entitys_manager.h"

#include <iostream>
#include <random>

#include "common/map_dto.h"
#include "common/shared_constants.h"
#include "common/snapshot.h"

const int16_t NEAR_CELLS = 3;
const int16_t COLLECTABLE_SPAWN_IT = 400;

EntityManager::EntityManager(Map& map_dto, uint8_t players_quantity) {
    for (int16_t i = 0; i < MAP_HEIGHT_BLOCKS; ++i) {
        for (int16_t j = 0; j < MAP_WIDTH_BLOCKS; ++j) {
            if (map_dto.blocks[i][j] != BlockType::Empty) {
                map_collisions.add_block(j * BLOCK_SIZE, i * BLOCK_SIZE);
            }
        }
    }
    std::vector<std::pair<int16_t, int16_t>> spawn_points = {
            {50, 50}, {MAP_WIDTH_PIXELS - 50, 50}, {50, 200}, {MAP_WIDTH_PIXELS - 50, 200}};
    for (uint8_t i = 0; i < players_quantity; ++i) {
        DuckPlayer player(map_collisions);
        player.set_coordenades_and_id(spawn_points[i].first, spawn_points[i].second, i);
        players.push_back(player);
    }

    Spawn spawn = {200, 200, 0, 50, true, 0};
    spawns.push_back(spawn);
    spawn = {250, 200, 1, 0, true, 0};
    spawns.push_back(spawn);
}

void EntityManager::process_action(action& action){
    DuckPlayer& player = players[action.duck_id];
    uint32_t id = player.update_status(action.command);
    if (id > 0) {
        for (auto &spawn : spawns) {
            if (spawn.collectable_id == id) {
                spawn.picked = true;
                spawn.collectable_id = 0;
                spawn.it_since_picked = 0;
                spawn.it_to_spawn = COLLECTABLE_SPAWN_IT + rand() % 100;
            }
        }
    }
}

void EntityManager::update_game_status() {
    for (auto& duck: players) {
        duck.move_duck();
    }
    // Actualizar la posicion de las balas y vida de los patos si les pegan
    verify_spawn();
}

GunType EntityManager::get_random_guntype() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, GunTypeCount - 1); // 0 es None
    return static_cast<GunType>(dis(gen));
}

void EntityManager::verify_spawn() {
    for (auto &spawn : spawns) {
        if (!spawn.picked)
            continue;

        if (spawn.it_since_picked > spawn.it_to_spawn) {
            uint32_t collectable_id = map_collisions.get_and_inc_collectable_id();

            Gun new_gun = { collectable_id, get_random_guntype(), spawn.x, spawn.y };
            map_collisions.add_gun(new_gun);
            spawn.collectable_id = collectable_id;
            spawn.it_since_picked = 0;
            spawn.picked = false;
        } else {
            ++spawn.it_since_picked;
        }
    }
}

void EntityManager::get_snapshot(Snapshot &snapshot) {
    for (auto duck : players) {
        snapshot.ducks.push_back(duck.get_status());
    }
    map_collisions.add_guns_to_snapshot(snapshot);
    for(auto const&[id, bullet] : bullets){
        snapshot.bullets.push_back(bullet);
    }
}

EntityManager::~EntityManager() {}
