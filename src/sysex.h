/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#ifndef SYSEX_h
#define SYSEX_h

#include <Arduino.h>
#include <EEPROM.h>

#include "definitions.h"
#include "storage.h"
#include "functions.h"

extern void processSysex(unsigned char*, unsigned int);

void setKnobAsCC(byte, byte, byte);

void setKnobAsCCWithChannel(byte, byte, byte, byte);

void setKnobAsDisabled(byte);

void setKnobAsNRPN(byte, byte, byte);

void useHighResolution(bool);

void handleChangeChannel(byte);

// Change preset on program change
extern void handleProgramChange(byte, byte);

#endif