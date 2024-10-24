#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <cstdint>

#include <sys/types.h>

#define MAX_DUCKS 4
#define MAX_PLAYER_NAME 32
// #define MAX_GUNS 32
// #define MAX_BULLETS 128
#define MAX_GUNS 1
#define MAX_BULLETS 1

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
    Sniper
};

struct Duck {
    uint8_t player_name_length;
    char player_name[MAX_PLAYER_NAME];

    uint8_t duck_id;  // del 0 al 3
    int8_t duck_hp;   // 100
    GunType gun;

    bool is_shooting;
    bool is_running;
    bool is_jumping;
    bool is_falling;
    bool is_flapping;
    bool facing_right;
    bool facing_up;
    bool is_laying;  // se esta haciendo el muerto
    bool is_dead;

    bool helment_equiped;
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

struct Snapshot {
    uint8_t players_quantity;
    Duck ducks[MAX_DUCKS];

    uint8_t guns_quantity;
    Gun guns[MAX_GUNS];

    uint8_t bullets_quantity;
    Bullet bullets[MAX_BULLETS];
} __attribute__((packed));


#endif
