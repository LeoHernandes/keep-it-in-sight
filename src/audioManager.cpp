#include "audioManager.h"
#define MINIAUDIO_IMPLEMENTATION
#define strcasecmp strcmp // Por algum motivo isso para de dar erro de compilação
#include <miniaudio.h>

Sound* AudioManager::walking_player_sound = NULL;
Sound* AudioManager::flash_camera_sound = NULL;

const float WALKING_SOUND_VOLUME = 0.5f;
const float FLASH_CAMERA_VOLUME = 0.15f;

static ma_engine engine;

struct Sound
{
    ma_sound audio;
};

void AudioManager::Init()
{
    ma_result result;
    result = ma_engine_init(NULL, &engine);

    if (result != MA_SUCCESS)
    {
        printf("Failed to initialize audio engine.");
        exit(1);
    }

    walking_player_sound = MakeSound("../../data/player_walking.mp3", true, WALKING_SOUND_VOLUME);
    flash_camera_sound = MakeSound("../../data/camera_flash.mp3", false, FLASH_CAMERA_VOLUME);
}

void AudioManager::Destroy()
{
    ma_engine_uninit(&engine);
}

Sound* AudioManager::MakeSound(const char* file_name, bool loop, float volume, bool stream)
{
    Sound *sound = (Sound*) malloc(sizeof(Sound));

    if (stream)
        ma_sound_init_from_file(&engine, file_name, MA_SOUND_FLAG_STREAM, NULL, NULL, &sound->audio);
    else
        ma_sound_init_from_file(&engine, file_name, 0, NULL, NULL, &sound->audio);

    ma_sound_set_looping(&sound->audio, loop);
    ma_sound_set_volume(&sound->audio, volume);

    return sound;
}

void AudioManager::DestroySound(Sound* sound)
{
    ma_sound_uninit(&sound->audio);
    free(sound);
}

void AudioManager::SetAudioVolume(Sound *sound, float volume)
{
    ma_sound_set_volume(&sound->audio, volume);
}

void AudioManager::PlayAudio(Sound *sound)
{
    ma_sound_start(&sound->audio);
}

void AudioManager::StopAudio(Sound *sound)
{
    ma_sound_stop(&sound->audio);
}

bool AudioManager::IsAudioPlaying(Sound* sound)
{
    return ma_sound_is_playing(&sound->audio);
}