#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "SDL2pp/Chunk.hh"
#include "SDL2pp/Music.hh"
#include "common/snapshot.h"

class SoundManager {
private:
    SDL2pp::Mixer mixer;
    std::unordered_map<GunType, SDL2pp::Chunk*> gun_sounds;
    std::unordered_map<std::string, SDL2pp::Chunk*> other_sounds;
    uint it_since_last_beep;
    SDL2pp::Music background_music;

public:
    SoundManager();

    void shoot_sound(GunType gun);

    void active_grenade_sound();

    void dead_sound();

    void revive_sound();

    void toggle_mute();

    void update();

    ~SoundManager();
};

#endif
