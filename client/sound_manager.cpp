#include "sound_manager.h"

#define CHANNELS 8

SoundManager::SoundManager():
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
    mixer.SetMusicVolume(0);
    mixer.PlayMusic(background_music);
    mixer.PauseMusic();

    for (int channel = 0; channel < CHANNELS; channel++) {
        mixer.SetVolume(channel, 0);
    }
}

void SoundManager::shoot_sound(GunType gun) {
    if (gun_sounds.find(gun) == gun_sounds.end()) {
        return;
    }

    try {
        mixer.PlayChannel(-1, *gun_sounds[gun]);
    } catch (const SDL2pp::Exception& e) {
        // No quedan canales
    }
}

void SoundManager::toggle_mute() {
    if (mixer.GetMusicVolume() == 0) {
        mixer.SetMusicVolume(5);
        mixer.ResumeMusic();
        for (int channel = 0; channel < CHANNELS; channel++) {
            mixer.SetVolume(channel, 7);
        }
    } else {
        mixer.SetMusicVolume(0);
        mixer.PauseMusic();

        for (int channel = 0; channel < CHANNELS; channel++) {
            mixer.SetVolume(channel, 0);
        }
    }
}

SoundManager::~SoundManager() {
    for (auto& sound: gun_sounds) {
        delete sound.second;
    }
}
