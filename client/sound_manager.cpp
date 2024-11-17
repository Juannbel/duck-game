#include "sound_manager.h"

#include "common/config.h"

#define CHANNELS 8

#define BULLETS_GROUP 1
#define QUACK_GROUP 2
#define GRENADE_GROUP 3

static Config& config = Config::get_instance();

const static uint BEEP_INTERVAL = config.get_client_fps() / 2;

SoundManager::SoundManager():
        mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS, 4096),
        background_music(DATA_PATH "/sounds/background.mp3") {

    mixer.GroupChannels(1, 2, QUACK_GROUP);
    mixer.GroupChannel(3, GRENADE_GROUP);
    mixer.GroupChannels(4, 7, BULLETS_GROUP);
    gun_sounds[LaserRifle] = new SDL2pp::Chunk(DATA_PATH "/sounds/laser.wav");
    gun_sounds[PewPewLaser] = new SDL2pp::Chunk(DATA_PATH "/sounds/laser.wav");
    gun_sounds[Magnum] = new SDL2pp::Chunk(DATA_PATH "/sounds/magnum.wav");
    gun_sounds[Shootgun] = new SDL2pp::Chunk(DATA_PATH "/sounds/shotgun.wav");
    gun_sounds[Sniper] = new SDL2pp::Chunk(DATA_PATH "/sounds/sniper.wav");
    gun_sounds[CowboyPistol] = new SDL2pp::Chunk(DATA_PATH "/sounds/pistol.wav");
    gun_sounds[DuelingPistol] = new SDL2pp::Chunk(DATA_PATH "/sounds/pistol.wav");
    gun_sounds[Ak47] = new SDL2pp::Chunk(DATA_PATH "/sounds/ak47.wav");
    gun_sounds[Grenade] = new SDL2pp::Chunk(DATA_PATH "/sounds/explosion.wav");
    gun_sounds[ActiveGrenade] = new SDL2pp::Chunk(DATA_PATH "/sounds/explosion.wav");
    other_sounds["beep"] = new SDL2pp::Chunk(DATA_PATH "/sounds/beep.wav");
    other_sounds["quack"] = new SDL2pp::Chunk(DATA_PATH "/sounds/quack.wav");

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
        int channel = mixer.GetGroupAvailableChannel(BULLETS_GROUP);
        mixer.PlayChannel(channel, *gun_sounds[gun]);
    } catch (const SDL2pp::Exception& e) {
        // No quedan canales
    }
}

void SoundManager::update() { it_since_last_beep++; }

void SoundManager::active_grenade_sound() {
    if (it_since_last_beep < BEEP_INTERVAL)
        return;
    try {
        int channel = mixer.GetGroupAvailableChannel(GRENADE_GROUP);
        mixer.PlayChannel(channel, *other_sounds["beep"]);
        it_since_last_beep = 0;
    } catch (const SDL2pp::Exception& e) {
        // No quedan canales
    }
}

void SoundManager::dead_sound() {
    try {
        int channel = mixer.GetGroupAvailableChannel(QUACK_GROUP);
        mixer.PlayChannel(channel, *other_sounds["quack"]);
    } catch (const SDL2pp::Exception& e) {
        // No quedan canales
    }
}

void SoundManager::toggle_mute() {
    if (mixer.GetMusicVolume() == 0) {
        mixer.SetMusicVolume(2);
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
