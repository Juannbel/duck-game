#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <cstring>
#include <string>
#include <vector>

#include "../common/lobby.h"
#include "../common/snapshot.h"

bool check_ducks(const std::vector<Duck>& ducks);
bool check_guns(const std::vector<Gun>& guns);
bool check_bullets(const std::vector<Bullet>& bullets);
bool check_boxes(const std::vector<Box>& boxes);
bool check_maps(const std::vector<MapDto>& maps);
bool check_game_info(const GameInfo& game_info, int32_t game_id, uint8_t duck_id_1,
                     uint8_t duck_id_2);
bool check_lobby_info(LobbyInfo info, int32_t game_id, uint8_t connected_players,
                      const std::string& name);

LobbyInfo initialize_lobby_info(int32_t game_id, int32_t connected_players,
                                const std::string& creator);
Snapshot initialize_snapshot();
Duck initialize_duck();
Gun initialize_gun();
Bullet initialize_bullet();
Box initialize_box();
MapDto initialize_map();

#endif
