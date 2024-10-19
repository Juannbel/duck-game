#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <cstdint>

#define MAX_DUCKS 4


enum GunType {
    Grenade = 0,
    Banana = 1,
    PewPewLaser = 2,
    LaserRifle = 3,
    Ak47 = 4,
    DuelingPistol = 5,
    CowboyPistol = 6,
    Magnum = 7,
    Shootgun = 8,
    Sniper = 9
};

struct Duck {
    uint8_t duck_id; // del 0 al 3
    uint8_t duck_hp; // 100

    GunType gun;

    bool is_shooting;
    bool is_running;
    bool is_jumping;
    bool is_flapping;
    bool facing_right;
    bool facing_up;
    bool is_lying;

    bool helment_equiped;
    bool armor_equiped;

    int32_t x;
    int32_t y;

} __attribute__((packed));

// armas tiradas
struct Gun {
    GunType type;
    int32_t x;
    int32_t y;
} __attribute__((packed));

struct Bullet {
    int32_t x;
    int32_t y;

    GunType type;
    bool facing_right;
} __attribute__((packed));

struct Snapshot {
    uint8_t players_quantity;
    Duck ducks[MAX_DUCKS];
} __attribute__((packed));


#endif