#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <map>
#include <string>

#include "common/snapshot.h"

class Config {
private:
    Config() { load_config(); }

    int client_fps;
    std::string window_title;
    int window_width;
    int window_height;
    uint8_t non_solid_opacity;
    uint8_t first_gamepad_player;
    float stick_dead_zone;
    float trigger_dead_zone;
    bool show_hud;

    int server_ticks;
    uint8_t rounds_to_win;
    uint8_t rounds_between_stats;
    bool cheats;
    uint8_t initial_duck_hp;
    float duck_speed;
    float fall_spped;

    int initial_box_hp;


    std::map<std::string, GunType> string_to_gun{{"none", None},
                                                 {"grenade", Grenade},
                                                 {"banana", Banana},
                                                 {"pew_pew_laser", PewPewLaser},
                                                 {"laser_rifle", LaserRifle},
                                                 {"ak47", Ak47},
                                                 {"dueling_pistol", DuelingPistol},
                                                 {"cowboy_pistol", CowboyPistol},
                                                 {"magnum", Magnum},
                                                 {"shootgun", Shootgun},
                                                 {"sniper", Sniper},
                                                 {"helmet", Helmet},
                                                 {"armor", Armor},
                                                 {"active_grenade", ActiveGrenade},
                                                 {"active_banana", ActiveBanana},
                                                 {"death_laser", DeathLaser}};


    std::map<GunType, uint8_t> gun_ammo;
    std::map<GunType, uint16_t> gun_recoil;
    std::map<GunType, float> gun_knockback;
    std::map<GunType, uint8_t> bullet_range;
    std::map<GunType, uint8_t> bullet_damage;
    std::map<GunType, float> bullet_speed;

public:
    static Config& get_instance() {
        static Config instance;
        return instance;
    }

    void load_config();

    // Client
    std::string get_window_title();
    int get_window_width();
    int get_window_height();
    int get_client_fps();
    uint8_t get_non_solid_opacity();
    uint8_t get_first_gamepad_player();
    float get_stick_dead_zone();
    float get_trigger_dead_zone();
    bool get_show_hud();

    // Server
    int get_server_ticks();
    uint8_t get_rounds_to_win();
    uint8_t get_rounds_between_stats();
    bool get_cheats_on();
    uint8_t get_initial_duck_hp();
    float get_duck_speed();
    float get_fall_speed();
    int get_initial_box_hp();
    uint8_t get_bullet_damage(GunType gun);
    uint8_t get_bullet_range(GunType gun);
    float get_bullet_speed(GunType gun);
    uint8_t get_gun_ammo(GunType gun);
    uint16_t get_gun_recoil(GunType gun);
    float get_gun_knockback(GunType gun);
};

#endif
