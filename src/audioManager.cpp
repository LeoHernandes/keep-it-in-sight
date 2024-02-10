#include "audioManager.h"
#define MINIAUDIO_IMPLEMENTATION
#define strcasecmp strcmp // Por algum motivo isso para de dar erro de compilação
#include <miniaudio.h>

Sound* AudioManager::step_player_sound = NULL;
Sound* AudioManager::flash_camera_sound = NULL;
Sound* AudioManager::ambient_sound = NULL;
Sound* AudioManager::door_sound = NULL;
Sound* AudioManager::tired_player_sound = NULL;
Sound* AudioManager::monster_sound = NULL;
Sound* AudioManager::coin_sound = NULL;
Sound* AudioManager::fear_monster_sound = NULL;

const float AudioManager::WALK_SOUND_VOLUME = 0.50f;
const float AudioManager::WALK_VELOCITY_AUDIO = 1.0f;
const float AudioManager::RUN_SOUND_VOLUME = 0.65f;
const float AudioManager::RUN_VELOCITY_AUDIO = 2.0f;

const float FLASH_CAMERA_VOLUME = 0.15f;
const float AMBIENT_SOUND_VOLUME = 0.50f;
const float DOOR_SOUND_VOLUME = 0.50f;
const float TIRED_PLAYER_VOLUME = 1.75f;
const float MONSTER_VOLUME = 0.50f;
const float FEAR_MONSTER_VOLUME = 0.20f;
const float COIN_VOLUME = 0.15f;

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

    step_player_sound = MakeSound("../../data/player_walking.mp3", true, WALK_SOUND_VOLUME);
    flash_camera_sound = MakeSound("../../data/camera_flash.mp3", false, FLASH_CAMERA_VOLUME);
    ambient_sound = MakeSound("../../data/backrooms_ambience.mp3", true, AMBIENT_SOUND_VOLUME);
    door_sound = MakeSound("../../data/door.mp3", false, DOOR_SOUND_VOLUME);
    tired_player_sound = MakeSound("../../data/heavy_breathing.mp3", false, TIRED_PLAYER_VOLUME);
    monster_sound = MakeSound("../../data/angry_beast.mp3", true, MONSTER_VOLUME);
    coin_sound = MakeSound("../../data/coin.mp3", false, COIN_VOLUME);
    fear_monster_sound = MakeSound("../../data/beast_roar.mp3", false, FEAR_MONSTER_VOLUME);
}

void AudioManager::Destroy()
{
    DestroySound(step_player_sound);
    DestroySound(flash_camera_sound);
    DestroySound(ambient_sound);
    DestroySound(door_sound);
    DestroySound(tired_player_sound);
    DestroySound(monster_sound);
    DestroySound(coin_sound);
    DestroySound(fear_monster_sound);

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

void AudioManager::SetPositionAudio(Sound *sound, glm::vec4 position)
{
    ma_sound_set_position(&sound->audio, position.x, position.y, position.z);
}

void AudioManager::SetAudioSpeed(Sound *sound, float speed)
{
    ma_sound_set_pitch(&sound->audio, speed);
}

void AudioManager::PlayAudio(Sound *sound)
{
    ma_sound_start(&sound->audio);
}

void AudioManager::PlayAudioWithRestart(Sound *sound)
{
    if (IsAudioPlaying(sound))
    {
        ma_sound_stop(&sound->audio);
        ma_sound_seek_to_pcm_frame(&sound->audio, 0);
    }
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