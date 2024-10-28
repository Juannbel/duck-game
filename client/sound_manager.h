#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <unordered_map>
#include <SDL2pp/SDL2pp.hh>
#include "SDL2pp/Chunk.hh"
#include "SDL2pp/Music.hh"
#include "common/snapshot.h"

class SoundManager {
private:
    SDL2pp::Mixer mixer;
    std::unordered_map<GunType, SDL2pp::Chunk*> gun_sounds;
    SDL2pp::Music background_music;

public:
    explicit SoundManager();

    void shoot_sound(GunType gun);

    void toggle_mute();

    ~SoundManager();
};

#endif
