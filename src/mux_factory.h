/*
  N32B Hi Res Firmware v3.6.0
  MIT License

  Copyright (c) 2022 SHIK
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
  void init(const uint8_t &, const uint8_t &, const uint8_t &, const uint8_t &);
  void setSignalPin(const bool &, const uint8_t &);
  void update(uint8_t &);
  void setMultiplexer(uint8_t &);
  uint16_t read(uint8_t &);

private:
  uint8_t channels[4];
  uint8_t signalPin[2];
};

#endif