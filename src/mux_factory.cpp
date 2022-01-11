/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#include "mux_factory.h"

MUX_FACTORY::MUX_FACTORY() {}
void MUX_FACTORY::init(uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t channel4)
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
    // timeout = millis();
}
void MUX_FACTORY::setSignalPin(bool muxIndex, uint8_t pin)
{
    signalPin[muxIndex] = pin;
    pinMode(pin, INPUT);
}

void MUX_FACTORY::update(uint8_t currentKnob)
{
    // if (millis() - timeout >= 1)
    // {
    setMultiplexer(currentKnob);
    for (uint8_t i = 3; i > 0; i--)
    {
        knobBuffer[currentKnob][i] = knobBuffer[currentKnob][i - 1];
    }
    knobBuffer[currentKnob][0] = readSingle(currentKnob);

    // timeout = millis();
    // }
}

uint16_t MUX_FACTORY::readSingle(uint8_t currentKnob)
{
    uint8_t bitReducer = activePreset.highResolution ? 0 : 3;
    bool pinSelector = currentKnob > 15 ? 1 : 0;

    return analogRead(signalPin[pinSelector]) >> bitReducer;
}

void MUX_FACTORY::setMultiplexer(uint8_t currentKnob)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        digitalWrite(channels[i], bitRead(currentKnob, i));
    }
}
