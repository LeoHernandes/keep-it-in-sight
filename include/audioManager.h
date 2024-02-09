#ifndef _AUDIO_H
#define _AUDIO_H

#include <glm/vec4.hpp>

struct Sound;

class AudioManager
{
private:
    AudioManager();
    
public:
    static const float WALK_SOUND_VOLUME;
    static const float WALK_VELOCITY_AUDIO;
    static const float RUN_SOUND_VOLUME;
    static const float RUN_VELOCITY_AUDIO;

    static Sound *step_player_sound;
    static Sound *flash_camera_sound;
    static Sound *ambient_sound;
    static Sound *door_sound;
    static Sound *tired_player_sound;
    static Sound *monster_sound;
    static Sound *coin_sound;

    static void Init();
    static void Destroy();

    static Sound* MakeSound(const char* file_name, bool loop = false, float volume = 1, bool stream = false);
    static void DestroySound(Sound *sound);
    
    static void SetAudioVolume(Sound *sound, float volume);
    static void SetAudioSpeed(Sound *sound, float speed);
    static void SetPositionAudio(Sound *sound, glm::vec4 position);
    static void PlayAudio(Sound *sound);
    static void PlayAudioWithRestart(Sound *sound);
    static void StopAudio(Sound *sound);
    static bool IsAudioPlaying(Sound *sound);
};

#endif