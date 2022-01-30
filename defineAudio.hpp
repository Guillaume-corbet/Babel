/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-guillaume.corbet
** File description:
** defineAudio
*/

#ifndef DEFINEAUDIO_HPP_
#define DEFINEAUDIO_HPP_

/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) 
#define WRITE_TO_FILE   (0)
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

#define SAMPLE_RATE (48000)
#define FRAMES_PER_BUFFER (480)
#define NUM_SECONDS (5)
#define NUM_CHANNELS (2)
#define TOTAL_FRAMES (NUM_SECONDS * SAMPLE_RATE)
#define NUM_SAMPLES (TOTAL_FRAMES * NUM_CHANNELS)
#define NUM_BYTES (NUM_SAMPLES * sizeof(SAMPLE))

#endif /* !DEFINEAUDIO_HPP_ */