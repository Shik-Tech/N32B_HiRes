/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#include <Arduino.h>

#include "definitions.h"
#include "functions.h"

#ifndef MUX_FACTORY_h
#define MUX_FACTORY_h

class MUX_FACTORY
{
public:
  MUX_FACTORY();
  void init(uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t channel4);
  void setSignalPin(bool muxIndex, uint8_t pin);
  void update(uint8_t);
  void setMultiplexer(uint8_t);
  uint8_t getKnobValue(uint8_t, bool);
  uint16_t readSingle(uint8_t);

private:
  uint8_t channels[4];
  uint8_t signalPin[2];
};

#endif