/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-guillaume.corbet
** File description:
** Codec
*/

#ifndef CODEC_HPP_
#define CODEC_HPP_

#include <iostream>
#include <opus.h>
#include <vector>
#include "defineAudio.hpp"


class Codec {
    public:
        Codec();
        ~Codec();
        void encode(std::vector<unsigned short> captured);
        void decode();
        std::vector<unsigned short> getDecoded() const;
        std::vector<unsigned char> getEncoded() const;
    protected:
    private:
        int _error;
        opus_int32 _enc_bytes;
        opus_int32 _dec_bytes;
        OpusEncoder *_encoder;
        OpusDecoder *_decoder;
        std::vector<unsigned short> _decoded;
        std::vector<unsigned char> _encoded;
};

#endif /* !CODEC_HPP_ */