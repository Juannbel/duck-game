#include "config.h"
#include <iostream>

#include <yaml-cpp/yaml.h>

void Config::load_config() try {
    YAML::Node config = YAML::LoadFile(CONFIG_PATH);

    client_fps = config["client_fps"].as<int>();
    window_width = config["window_width"].as<int>();
    window_height = config["window_height"].as<int>();

    server_ticks = config["server_ticks"].as<int>();
    cheats = config["cheats"].as<bool>();
    initial_duck_hp = config["initial_duck_hp"].as<int>();
    duck_speed = config["duck_speed"].as<float>();
    fall_spped = config["fall_speed"].as<float>();

    initial_box_hp = config["initial_box_hp"].as<int>();

    YAML::Node guns = config["guns"];

    for (auto gun : guns) {
        GunType type = string_to_gun[gun.first.as<std::string>()];
        gun_ammo[type] = gun.second["ammo"].as<int>();
        gun_recoil[type] = gun.second["recoil"].as<int>();
        bullet_damage[type] = gun.second["damage"].as<int>();
        bullet_range[type] = gun.second["range"].as<int>();
        bullet_speed[type] = gun.second["speed"].as<float>();
    }
} catch (YAML::Exception& e) {
    std::cerr << "Error loading config file: " << e.what() << std::endl;
    exit(1);
}

int Config::get_client_fps() { return client_fps; }

int Config::get_window_width() { return window_width; }

int Config::get_window_height() { return window_height; }

int Config::get_server_ticks() { return server_ticks; }

bool Config::get_cheats_on() { return cheats; }

uint8_t Config::get_initial_duck_hp() { return initial_duck_hp; }

float Config::get_duck_speed() { return duck_speed; }

float Config::get_fall_speed() { return fall_spped; }

int Config::get_initial_box_hp() { return initial_box_hp; }

uint8_t Config::get_gun_ammo(GunType type) {
    if (!gun_ammo.count(type)) {
        return 0;
    }
    return gun_ammo[type];
}

uint16_t Config::get_gun_recoil(GunType type) {
    if (!gun_recoil.count(type)) {
        return 0;
    }
    return gun_recoil[type];
}

uint8_t Config::get_bullet_damage(GunType type) {
    if (!bullet_damage.count(type)) {
        return 0;
    }
    return bullet_damage[type];
}

uint8_t Config::get_bullet_range(GunType type) {
    if (!bullet_range.count(type)) {
        return 0;
    }
    return bullet_range[type];
}

float Config::get_bullet_speed(GunType type) { return bullet_speed[type]; }
