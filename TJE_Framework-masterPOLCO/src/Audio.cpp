#include <iostream>
#include "audio.h"


Audio::Audio(const char* filepath, bool loop, float volume) {
    hSample = 0;
    hSampleChannel = 0;
    LoadSample(filepath, loop, volume);
    printf("DEBUG: new audio instance\n");
}

Audio::~Audio() {
    BASS_SampleFree(hSample);
}


bool Audio::Init()
{
    // Initialize BASS
    if (BASS_Init(-1, 44100, 0, nullptr, nullptr))
        return true;
    else
        return false;
}


void Audio::LoadSample(const char* filepath, bool loop, float volume) {
    if (loop == true) {
        hSample = BASS_SampleLoad(false, filepath, 0, 0, 3, BASS_SAMPLE_LOOP);
    }
    else {
        hSample = BASS_SampleLoad(false, filepath, 0, 0, 3, 0);
    }

    if (hSample == 0) {
        // Error loading the audio sample
        // Print an error message or log the error
        printf("Error loading audio sample: %s\n", filepath);
    }
    else {
        hSampleChannel = BASS_SampleGetChannel(hSample, false);
        BASS_ChannelSetAttribute(hSampleChannel, BASS_ATTRIB_VOL, volume);
    }
}

void Audio::Play() {
    BASS_ChannelPlay(hSampleChannel, true);
}

void Audio::Stop() {
    BASS_ChannelStop(hSampleChannel);
}

void Audio::Pause() {
    BASS_ChannelPause(hSampleChannel);
}

void Audio::Resume() {
    BASS_ChannelPlay(hSampleChannel, false);
}