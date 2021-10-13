/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#ifndef STORAGE_h
#define STORAGE_h

#include <Arduino.h>
#include <EEPROM.h>

#include "definitions.h"

// Checks if the device is starting up for the first time
extern bool isEEPROMvalid();

extern void formatFactory();

// Loads the specified preset in the RAM and make it the last used preset
extern void loadPreset(uint8_t);

extern void savePreset(uint8_t);

#endif