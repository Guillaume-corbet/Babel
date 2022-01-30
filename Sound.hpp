/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Sound
*/

#ifndef SOUND_HPP_
#define SOUND_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <iostream>
#include <vector>
#include "defineAudio.hpp"

typedef struct
{
    int frameIndex;
    int maxFrameIndex;
    SAMPLE *recordedSamples;
} paTestData;

class Sound {
    public:
        Sound();
        ~Sound();
        PaError getSound();
        PaError playSound();
        PaError terminateStream();
        void init();
    protected:
    private:
        PaStreamParameters _inputParameters;
        PaStreamParameters _outputParameters;
        paTestData _data;
        PaError _error;
};

//int recordCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
//int playCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

#endif /* !SOUND_HPP_ */
