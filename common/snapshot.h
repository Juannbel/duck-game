#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <cstdint>
#include <vector>

#include <sys/types.h>

#include "map_dto.h"

#define MAX_DUCKS 4
#define MAX_PLAYER_NAME 16
#define MAX_GUNS 32
#define MAX_BULLETS 128

enum GunType {
    None,
    Grenade,
    Banana,
    PewPewLaser,
    LaserRifle,
    Ak47,
    DuelingPistol,
    CowboyPistol,
    Magnum,
    Shootgun,
    Sniper,
    Helmet,
    Armor,
    GunTypeCount,
    ActiveGrenade,
    ActiveBanana,
    DeathLaser,
};

struct Duck {
    uint8_t player_name_length;
    char player_name[MAX_PLAYER_NAME];

    uint8_t duck_id;  // del 0 al 3
    uint8_t duck_hp;
    GunType gun;
    uint8_t rounds_won = 0;

    bool is_shooting;
    bool is_running;
    bool is_jumping;
    bool is_falling;
    bool is_flapping;
    bool facing_right;
    bool facing_up;
    bool is_laying;  // se esta haciendo el muerto
    bool is_dead;

    bool helmet_equiped;
    bool armor_equiped;

    int16_t x;
    int16_t y;

} __attribute__((packed));

// armas tiradas
struct Gun {
    uint32_t gun_id;

    GunType type;
    int16_t x;
    int16_t y;
} __attribute__((packed));

struct Bullet {
    uint32_t bullet_id;

    int16_t x;
    int16_t y;
    uint16_t angle;  // 0 es hacia la derecha, 90 hacia arriba, 180 hacia la izquierda, 270 hacia
                     // abajo

    GunType type;
} __attribute__((packed));

enum BoxStatus { NoDamage, LittleDamage, MediumDamage, HeavyDamage };

struct Box {
    uint32_t box_id;
    int16_t x;
    int16_t y;
    BoxStatus status;
} __attribute__((packed));

struct Snapshot {
    // cppcheck-suppress unusedStructMember
    bool round_finished = true;
    // cppcheck-suppress unusedStructMember
    bool show_stats = false;
    // cppcheck-suppress unusedStructMember
    bool game_finished = false;
    std::vector<Duck> ducks;
    std::vector<Gun> guns;
    std::vector<Bullet> bullets;
    std::vector<Box> boxes;
    std::vector<MapDto> maps;
};


#endif
