/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-guillaume.corbet
** File description:
** Codec
*/

#include "Codec.hpp"

Codec::Codec()
{
    std::vector<unsigned short> dec(FRAMES_PER_BUFFER * NUM_CHANNELS);
    std::vector<unsigned char> enc(FRAMES_PER_BUFFER * NUM_CHANNELS * 2);

    this->_decoded = dec;
    this->_encoded = enc;
    this->_encoder = opus_encoder_create(SAMPLE_RATE, NUM_CHANNELS, OPUS_APPLICATION_AUDIO, &this->_error);
    if (this->_error != OPUS_OK) {
        std::cout << "Opus encoder error" << std::endl;
    }
    this->_decoder = opus_decoder_create(SAMPLE_RATE, NUM_CHANNELS, &this->_error);
    if (this->_error != OPUS_OK) {
        std::cout << "Opus decoder error" << std::endl;
    }
}

Codec::~Codec()
{
    opus_decoder_destroy(this->_decoder);
    opus_encoder_destroy(this->_encoder);
}

void Codec::encode(std::vector<unsigned short> captured)
{
    if ((this->_enc_bytes = opus_encode(this->_encoder, reinterpret_cast<opus_int16 const*>(
        captured.data()), 480, this->_encoded.data(), this->_encoded.size())) < 0)
    {
        std::cout << "opus_encode failed: " << this->_enc_bytes << std::endl;
    }
}

void Codec::decode()
{
    if ((this->_dec_bytes = opus_decode(this->_decoder, this->_encoded.data(), this->_enc_bytes,
        reinterpret_cast<opus_int16*>(this->_decoded.data()), 480, 0)) < 0)
    {
        std::cout << "opus_decode failed: " << this->_dec_bytes << std::endl;
    }
}

std::vector<unsigned short> Codec::getDecoded() const
{
    return (this->_decoded);
}

std::vector<unsigned char> Codec::getEncoded() const
{
    return (this->_encoded);
}