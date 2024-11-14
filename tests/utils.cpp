#include "utils.h"

bool check_ducks(const std::vector<Duck>& duck){
    int duck_index = 0;
    if(duck[duck_index].duck_id != 0) return false;
    if(duck[duck_index].duck_hp != 100) return false;
    if(duck[duck_index].gun != None) return false;
    if(duck[duck_index].rounds_won != 0) return false;
    if(duck[duck_index].is_shooting != false) return false;
    if(duck[duck_index].is_running != false) return false;
    if(duck[duck_index].is_jumping != false) return false;
    if(duck[duck_index].is_falling != false) return false;
    if(duck[duck_index].is_flapping != false) return false;
    if(duck[duck_index].facing_right != false) return false;
    if(duck[duck_index].facing_up != false) return false;
    if(duck[duck_index].is_laying != false) return false;
    if(duck[duck_index].is_dead != false) return false;
    if(duck[duck_index].helmet_equiped != false) return false;
    if(duck[duck_index].armor_equiped != false) return false;
    if(duck[duck_index].x != 1) return false;
    if(duck[duck_index].y != 1) return false;
    return true;
}

bool check_guns(const std::vector<Gun>& guns){
    int gun_index = 0;
    if(guns[gun_index].gun_id != 0) return false;
    if(guns[gun_index].type != Ak47) return false;
    if(guns[gun_index].x != 3) return false;
    if(guns[gun_index].y != 3) return false;
    return true;
}

bool check_bullets(const std::vector<Bullet>& bullets){
    int bullet_index = 0;
    if(bullets[bullet_index].bullet_id != 0) return false;
    if(bullets[bullet_index].x != 5) return false;
    if(bullets[bullet_index].y != 5) return false;
    if(bullets[bullet_index].angle != 90) return false;
    if(bullets[bullet_index].type != Ak47) return false;
    return true;
}

bool check_boxes(const std::vector<Box>& boxes){
    int box_index = 0;
    if(boxes[box_index].box_id != 0) return false;
    if(boxes[box_index].x != 7) return false;
    if(boxes[box_index].y != 7) return false;
    if(boxes[box_index].status != NoDamage) return false;
    return true;
}

bool check_maps(const std::vector<MapDto>& maps){
    int map_index = 0;
    if(maps[map_index].theme != 0) return false;
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++){
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++){
            if(maps[map_index].blocks[0][0].type != Empty) return false;
            if(maps[map_index].blocks[0][0].solid != false) return false;
        }
    }
    return true;
}

Snapshot initialize_snapshot(){
    Snapshot snapshot;
    snapshot.round_finished = false;
    snapshot.show_stats = false;
    snapshot.game_finished = false;

    std::vector<Duck> ducks;
    Duck duck = initialize_duck();
    ducks.push_back(duck);
    snapshot.ducks = ducks;

    std::vector<Gun> guns;
    Gun gun = initialize_gun();
    guns.push_back(gun);
    snapshot.guns = guns;

    std::vector<Bullet> bullets;
    Bullet bullet = initialize_bullet();
    bullets.push_back(bullet);
    snapshot.bullets = bullets;

    std::vector<Box> boxes;
    Box box = initialize_box();
    boxes.push_back(box);
    snapshot.boxes = boxes;


    std::vector<MapDto> maps;
    MapDto map = initialize_map();
    maps.push_back(map);
    snapshot.maps = maps;
    return snapshot;
}

Duck initialize_duck(){
    Duck duck;
    duck.duck_id = 0;
    duck.duck_hp = 100;
    duck.gun = None;
    duck.rounds_won = 0;
    duck.is_shooting = false;
    duck.is_running = false;
    duck.is_jumping = false;
    duck.is_falling = false;
    duck.is_flapping = false;
    duck.facing_right = false;
    duck.facing_up = false;
    duck.is_laying = false;
    duck.is_dead = false;
    duck.helmet_equiped = false;
    duck.armor_equiped = false;
    duck.x = 1;
    duck.y = 1;
    return duck;
}

Gun initialize_gun(){
    Gun gun;
    gun.gun_id = 0;
    gun.type = Ak47;
    gun.x = 3;
    gun.y = 3;
    return gun;
}

Bullet initialize_bullet(){
    Bullet bullet;
    bullet.bullet_id = 0;
    bullet.x = 5;
    bullet.y = 5;
    bullet.angle = 90;
    bullet.type = Ak47;
    return bullet;
}

Box initialize_box (){
    Box box;
    box.box_id = 0;
    box.x = 7;
    box.y = 7;
    box.status = NoDamage;
    return box;
}

MapDto initialize_map(){
    MapDto map;
    map.theme = 0;
    for (int i = 0; i < MAP_HEIGHT_BLOCKS; i++){
        for (int j = 0; j < MAP_WIDTH_BLOCKS; j++){
            map.blocks[i][j].type = Empty;
            map.blocks[i][j].solid = false;
        }
    }
    return map;
}
