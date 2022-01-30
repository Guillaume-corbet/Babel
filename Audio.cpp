/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Audio
*/

#include "Audio.hpp"

std::vector<unsigned char> buffer;

Audio::Audio()
{
}

Audio::~Audio()
{
}

int recCallBack(const void *tmp_buff, void *, unsigned long frm, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *)
{
    std::cout << "Record data sent !" << std::endl;
    buffer.assign(reinterpret_cast<const float *>(tmp_buff), reinterpret_cast<const float *>(tmp_buff) + frm * NUM_CHANNELS);
    std::cout << "buffer record" << buffer.size() << std::endl;
    return (0);
}

int listCallBack(const void *, void *tmp_buff, unsigned long frm, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *)
{
    std::cout << "Listening record data !" << std::endl;
    buffer.assign(reinterpret_cast<const float *>(tmp_buff), reinterpret_cast<const float *>(tmp_buff) + frm * NUM_CHANNELS);
    std::cout << "buffer play" << buffer.size() << std::endl;
    return (0);
}

void Audio::init()
{
    this->_error = Pa_Initialize();
    if (this->_error != paNoError) {
        fprintf(stderr, "Error: Initialize error.\n");
    }
    this->_inputParameters.device = Pa_GetDefaultInputDevice();
    if (this->_inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
    }
    this->_inputParameters.channelCount = 2;
    this->_inputParameters.sampleFormat = paFloat32;
    this->_inputParameters.suggestedLatency = Pa_GetDeviceInfo(this->_inputParameters.device)->defaultLowInputLatency;
    this->_inputParameters.hostApiSpecificStreamInfo = NULL;
    this->_outputParameters.device = Pa_GetDefaultOutputDevice();
    if (this->_outputParameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default output device.\n");
    }
    this->_outputParameters.channelCount = 2;
    this->_outputParameters.sampleFormat = paFloat32;
    this->_outputParameters.suggestedLatency = Pa_GetDeviceInfo(this->_outputParameters.device)->defaultLowOutputLatency;
    this->_outputParameters.hostApiSpecificStreamInfo = NULL;
    std::cout << "Port audio initialized" << std::endl;
}

void Audio::record()
{
    std::cout << "record audio" << std::endl;
    if (Pa_OpenStream(&this->_input, &this->_inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER,
        paClipOff, recCallBack, this) != paNoError) {
        std::cout << "Can't Pa_GetDefaultInputDevice" << std::endl;
        return;
    }
	if (Pa_StartStream(this->_input) != paNoError) {
        std::cout << "Can't Pa_StartStream" << std::endl;
        return;
    }
}

void Audio::stopRecord()
{
    std::cout << "stop record audio" << std::endl;
    if (Pa_CloseStream(this->_input) != paNoError) {
        std::cout << "Can't Pa_CloseStream" << std::endl;
        return;
    }
}

void Audio::playAudio()
{
    std::cout << "play audio" << std::endl;
    if (Pa_OpenStream(&this->_output, NULL, &this->_outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, listCallBack, this) != paNoError) {
        std::cout << "Can't Pa_OpenStream" << std::endl;
        return;
    }
	if (Pa_StartStream(this->_output) != paNoError) {
        std::cout << "Can't Pa_StartStream" << std::endl;
        return;
    }
}

void Audio::stopPlayAudio()
{
    std::cout << "stop play audio" << std::endl;
    if (Pa_CloseStream(this->_output) != paNoError) {
        std::cout << "Can't Pa_CloseStream" << std::endl;
        return;
    }
}