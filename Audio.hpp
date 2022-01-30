/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Audio
*/

#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <iostream>
#include <vector>
#include "defineAudio.hpp"

class Audio {
    public:
        Audio();
        ~Audio();
        void record();
        void stopRecord();
        void playAudio();
        void stopPlayAudio();
        PaError terminateStream();
        void init();
    protected:
    private:
        PaStreamParameters _inputParameters;
        PaStreamParameters _outputParameters;
        PaStream *_input;
        PaStream *_output;
        PaError _error;
};

#endif /* !AUDIO_HPP_ */
