#ifndef AUDIO_H
#define AUDIO_H


#include "extra/bass.h"
#include "utils.h"

class Audio {
public:
    Audio(const char* filepath, bool loop, float volume);
    ~Audio();

    static bool Init();

    void LoadSample(const char* filepath, bool loop, float volume);
    void Play();
    void Stop();
    void Pause();
    void Resume();

private:
    HSAMPLE hSample;
    HCHANNEL hSampleChannel;
};

#endif 