/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** Sound
*/

#include "Sound.hpp"

Sound::Sound()
{
}

Sound::~Sound()
{
}

int recordCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if (framesLeft < framesPerBuffer) {
        framesToCalc = framesLeft;
        finished = paComplete;
    } else {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if (inputBuffer == NULL) {
        for (i=0; i<framesToCalc; i++) {
            *wptr++ = SAMPLE_SILENCE;
            if (NUM_CHANNELS == 2) *wptr++ = SAMPLE_SILENCE;
        }
    } else {
        for (i=0; i<framesToCalc; i++) {
            *wptr++ = *rptr++;
            if ( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}

int playCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    paTestData *data = (paTestData*)userData;
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if (framesLeft < framesPerBuffer) {
        for (i=0; i<framesLeft; i++) {
            *wptr++ = *rptr++;
            if (NUM_CHANNELS == 2) *wptr++ = *rptr++;
        }
        for (; i<framesPerBuffer; i++) {
            *wptr++ = 0;
            if (NUM_CHANNELS == 2) *wptr++ = 0;
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    } else {
        for (i=0; i<framesPerBuffer; i++) {
            *wptr++ = *rptr++;
            if (NUM_CHANNELS == 2) *wptr++ = *rptr++;
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

PaError Sound::getSound()
{
    PaStream *stream;

    this->_data.frameIndex = 0;
    this->_data.recordedSamples = (SAMPLE *) malloc(NUM_BYTES);
    if (this->_data.recordedSamples == NULL) {
        printf("Could not allocate record array.\n");
        return (this->terminateStream());
    }
    for (int i = 0; i < (NUM_SAMPLES); i++) this->_data.recordedSamples[i] = 0;
    this->_error = Pa_OpenStream(&stream, &this->_inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, recordCallback, &this->_data);
    if (this->_error != paNoError)
        return (this->terminateStream());
    this->_error = Pa_StartStream(stream);
    if (this->_error != paNoError)
        return (this->terminateStream());
    printf("\n=== Now recording!! Please speak into the microphone. ===\n");
    fflush(stdout);
    while ((this->_error = Pa_IsStreamActive(stream)) == 1) {
        Pa_Sleep(100);
        printf("index = %d\n", this->_data.frameIndex );
        fflush(stdout);
    }
    this->_error = Pa_CloseStream(stream);
    if (this->_error != paNoError)
        return (this->terminateStream());
    return (this->_error);
}

PaError Sound::playSound()
{
    PaStream *stream;

    this->_data.frameIndex = 0;
    printf("\n=== Now playing back. ===\n");
    fflush(stdout);
    this->_error = Pa_OpenStream(&stream, NULL, &this->_outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, playCallback, &this->_data);
    if (this->_error != paNoError)
        return (this->terminateStream());
    if (stream) {
        this->_error = Pa_StartStream(stream);
        if (this->_error != paNoError)
            return (this->terminateStream());
        printf("Waiting for playback to finish.\n");
        fflush(stdout);
        while ((this->_error = Pa_IsStreamActive(stream)) == 1) {
            Pa_Sleep(100);
            printf("index = %d\n", this->_data.frameIndex );
            fflush(stdout);
        }
        if (this->_error < 0)
            return (this->terminateStream());
        this->_error = Pa_CloseStream(stream);
        if (this->_error != paNoError)
            return (this->terminateStream());
        printf("Done.\n");
        fflush(stdout);
    }
    std::cout << "end play" << std::endl;
    this->terminateStream();
    return (this->_error);
}

PaError Sound::terminateStream()
{
    if (this->_data.recordedSamples != NULL) {
        free(this->_data.recordedSamples);
        std::cout << "free data good" << std::endl;
    }
    if (this->_error != paNoError) {
        fprintf(stderr, "An error occured while using the portaudio stream\n");
        fprintf(stderr, "Error number: %d\n", this->_error);
        fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(this->_error));
        this->_error = 1;
    }
    return (this->_error);
}

void Sound::init()
{
    this->_data.maxFrameIndex = TOTAL_FRAMES;
    this->_data.frameIndex = 0;
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