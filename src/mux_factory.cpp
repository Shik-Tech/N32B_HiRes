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
}
void MUX_FACTORY::setSignalPin(bool muxIndex, uint8_t pin)
{
    signalPin[muxIndex] = pin;
    pinMode(pin, INPUT);
}

void MUX_FACTORY::update(uint8_t currentKnob)
{
    setMultiplexer(currentKnob);
    for (uint8_t i = 3; i > 0; i--)
    {
        knobBuffer[currentKnob][0][i] = knobBuffer[currentKnob][0][i - 1];
        knobBuffer[currentKnob][1][i] = knobBuffer[currentKnob][1][i - 1];
    }
    uint16_t shiftedValue = map(readSingle(currentKnob), 0, 1023, 0, 16383);
    knobBuffer[currentKnob][0][0] = shiftedValue >> 7;          // MSB
    knobBuffer[currentKnob][1][0] = lowByte(shiftedValue) >> 1; // LSB
}

uint16_t MUX_FACTORY::readSingle(uint8_t currentKnob)
{
    bool pinSelector = currentKnob > 15 ? 1 : 0;
    return analogRead(signalPin[pinSelector]);
}

void MUX_FACTORY::setMultiplexer(uint8_t currentKnob)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        digitalWrite(channels[i], bitRead(currentKnob, i));
    }
}

uint8_t MUX_FACTORY::getKnobValue(uint8_t index, bool isMSB)
{
    uint16_t average = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        average += knobBuffer[index][isMSB][i];
    }
    average /= 4;
    return average;
}