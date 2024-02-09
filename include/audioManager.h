#ifndef _AUDIO_H
#define _AUDIO_H

struct Sound;

class AudioManager
{
private:
    AudioManager();
    
public:
    static Sound *walking_player_sound;
    static Sound* flash_camera_sound;

    static void Init();
    static void Destroy();

    static Sound* MakeSound(const char* file_name, bool loop = false, float volume = 1, bool stream = false);
    static void DestroySound(Sound *sound);
    
    static void SetAudioVolume(Sound *sound, float volume);
    static void PlayAudio(Sound *sound);
    static void StopAudio(Sound *sound);
    static bool IsAudioPlaying(Sound *sound);
};

#endif