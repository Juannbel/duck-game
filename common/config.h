#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <map>
#include <string>

#include "common/snapshot.h"

class Config {
private:
    static int client_fps;
    static int window_width;
    static int window_height;

    static int server_ticks;
    static uint8_t initial_duck_hp;
    static float duck_speed;
    static float fall_spped;

    static int initial_box_hp;

    static std::map<std::string, GunType> string_to_gun;

    static std::map<GunType, uint8_t> gun_ammo;
    static std::map<GunType, uint16_t> gun_recoil;
    static std::map<GunType, uint8_t> bullet_range;
    static std::map<GunType, uint8_t> bullet_damage;
    static std::map<GunType, float> bullet_speed;

    Config() {};

public:
    static void load_config();

    // Client
    static int get_window_width();
    static int get_window_height();
    static int get_client_fps();

    // Server
    static int get_server_ticks();
    static uint8_t get_initial_duck_hp();
    static float get_duck_speed();
    static float get_fall_speed();
    static int get_initial_box_hp();
    static uint8_t get_bullet_damage(GunType gun);
    static uint8_t get_bullet_range(GunType gun);
    static float get_bullet_speed(GunType gun);
    static uint8_t get_gun_ammo(GunType gun);
    static uint16_t get_gun_recoil(GunType gun);

};

#endif
