#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <cstring>
#include <vector>

#include "../common/snapshot.h"

bool check_ducks(const std::vector<Duck>& ducks);
bool check_guns(const std::vector<Gun>& guns);
bool check_bullets(const std::vector<Bullet>& bullets);
bool check_boxes(const std::vector<Box>& boxes);
bool check_maps(const std::vector<MapDto>& maps);
Snapshot initialize_snapshot();
Duck initialize_duck();
Gun initialize_gun();
Bullet initialize_bullet();
Box initialize_box();
MapDto initialize_map();

#endif
