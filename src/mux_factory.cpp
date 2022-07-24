/*
  N32B Hi Res Firmware v3.6.0
  MIT License

  Copyright (c) 2022 SHIK
*/

#include "mux_factory.h"

MUX_FACTORY::MUX_FACTORY() {}
void MUX_FACTORY::init(const uint8_t &channel1, const uint8_t &channel2, const uint8_t &channel3, const uint8_t &channel4)
{
    channels[0] = channel1;
    channels[1] = channel2;
    channels[2] = channel3;
    channels[3] = channel4;
    for (uint8_t i = 0; i < 4; i++)
    {
        pinMode(channels[i], OUTPUT);
    }
    delay(10);
}
void MUX_FACTORY::setSignalPin(const bool &muxIndex, const uint8_t &pin)
{
    signalPin[muxIndex] = pin;
    pinMode(pin, INPUT);
}

void MUX_FACTORY::update(uint8_t &currentKnob)
{
    setMultiplexer(currentKnob);
    knobValues[currentKnob][0] = (EMA_a * read(currentKnob)) + ((1 - EMA_a) * knobValues[currentKnob][0]);
}

uint16_t MUX_FACTORY::read(uint8_t &currentKnob)
{
    bool pinSelector = currentKnob > 15 ? 1 : 0;
    return analogRead(signalPin[pinSelector]);
}

void MUX_FACTORY::setMultiplexer(uint8_t &currentKnob)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        digitalWrite(channels[i], bitRead(currentKnob, i));
    }
}