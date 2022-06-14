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

bool isEEPROMvalid();
void formatFactory();
void loadPreset(uint8_t);
void savePreset(uint8_t);

const uint8_t knobsLocation[] PROGMEM = {
    15, 14, 12, 9, 31, 24, 20, 16,
    13, 11, 10, 2, 30, 25, 21, 17,
    8, 7, 4, 1, 29, 26, 22, 18,
    6, 5, 3, 0, 28, 27, 23, 19};

#endif