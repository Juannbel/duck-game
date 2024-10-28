#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <unordered_map>
#include <SDL2pp/SDL2pp.hh>
#include "SDL2pp/Chunk.hh"
#include "SDL2pp/Music.hh"
#include "common/snapshot.h"

#define CHANNELS 8
class SoundManager {
    private:
    SDL2pp::Mixer mixer;
    std::unordered_map<GunType, SDL2pp::Chunk*> gun_sounds;
    SDL2pp::Music background_music;

    public:
    SoundManager() :
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, 4096),
        background_music(DATA_PATH "/sounds/background.mp3") {
            gun_sounds[LaserRifle] = new SDL2pp::Chunk(DATA_PATH "/sounds/laser.wav");
            gun_sounds[PewPewLaser] = new SDL2pp::Chunk(DATA_PATH "/sounds/laser.wav");
            gun_sounds[Magnum] = new SDL2pp::Chunk(DATA_PATH "/sounds/magnum.wav");
            gun_sounds[Shootgun] = new SDL2pp::Chunk(DATA_PATH "/sounds/shotgun.wav");
            gun_sounds[Sniper] = new SDL2pp::Chunk(DATA_PATH "/sounds/sniper.wav");
            gun_sounds[CowboyPistol] = new SDL2pp::Chunk(DATA_PATH "/sounds/pistol.wav");
            gun_sounds[DuelingPistol] = new SDL2pp::Chunk(DATA_PATH "/sounds/pistol.wav");
            gun_sounds[Ak47] = new SDL2pp::Chunk(DATA_PATH "/sounds/ak47.wav");
            mixer.SetMusicVolume(5);
            mixer.PlayMusic(background_music);
            for (int channel = 0; channel < CHANNELS; channel++) {
                mixer.SetVolume(channel, 5);
            }
        }

    void shoot_sound(GunType gun) {
        if (gun_sounds.find(gun) == gun_sounds.end()) {
            return;
        }

        try {
            mixer.PlayChannel(-1, *gun_sounds[gun]);
        } catch (const SDL2pp::Exception& e) {
            // No quedan canales
        }
    }

    void toggle_mute() {
        if (mixer.IsMusicPlaying()) {
            mixer.PauseMusic();
        } else {
            mixer.ResumeMusic();
        }

        if (mixer.GetMusicVolume() == 0) {
            mixer.SetMusicVolume(5);
        } else {
            mixer.SetMusicVolume(0);
        }

        for (int channel = 0; channel < CHANNELS; channel++) {
            if (mixer.GetVolume(channel) == 0) {
                mixer.SetVolume(channel, 5);
            } else {
                mixer.SetVolume(channel, 0);
            }
        }
    }

    ~SoundManager() {
        toggle_mute();
        for (auto& sound : gun_sounds) {
            delete sound.second;
        }
    }
};

#endif
