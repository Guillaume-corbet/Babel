/*
** EPITECH PROJECT, 2020
** cpp_babel_2020
** File description:
** main
*/

#include "Codec.hpp"
#include "Audio.hpp"
#include "Sound.hpp"
#include <unistd.h>

int main(void)
{
    //Audio *audio = new Audio();

    //audio->init();
        /*audio->record();
        usleep(1000000);
        audio->stopRecord();
        usleep(1000000);
        audio->playAudio();
        usleep(1000000);
        audio->stopPlayAudio();*/
    while (1) {
        Sound *sound = new Sound();
        sound->init();
        sound->getSound();
        sound->playSound();
        delete(sound);
    }
    return (0);
}