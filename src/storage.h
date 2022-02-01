/*
  N32B Hi Res Firmware v3.5.2
  MIT License

  Copyright (c) 2022 SHIK
*/

#ifndef STORAGE_h
#define STORAGE_h

#include <Arduino.h>
#include <EEPROM.h>

#include "definitions.h"

extern bool isEEPROMvalid();
extern void formatFactory();
extern void loadPreset(uint8_t);
extern void savePreset(uint8_t);

#endif