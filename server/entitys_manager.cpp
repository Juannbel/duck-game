#include "entitys_manager.h"

#include <iostream>

#include "common/map_dto.h"
#include "common/shared_constants.h"

const int16_t NEAR_CELLS = 3;

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
}

void EntityManager::process_action(action& action){
    DuckPlayer& player = players[action.duck_id];
    player.update_status(action.command);
}

void EntityManager::update_game_status() {
    for (auto& duck: players) {
        duck.move_duck();
    }
    // Actualizar la posicion de las balas y vida de los patos si les pegan
    // Actualizar spawns y agregar armas nuevas a map_collisions
}

void EntityManager::get_snapshot(Snapshot &snapshot) {
    int i = 0;
    for (auto duck : players) {
        snapshot.ducks[i] = duck.get_status();
        ++i;
    }
    snapshot.players_quantity = i;
    map_collisions.add_guns_to_snapshot(snapshot);
    i = 0;
    for(auto const&[id, bullet] : bullets){
        snapshot.bullets[i] = bullet;
        ++i;
    }
    snapshot.bullets_quantity = i;
}

EntityManager::~EntityManager() {}
