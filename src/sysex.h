/*
  N32B Hi Res Firmware v3.5.2
  MIT License

  Copyright (c) 2022 SHIK
*/

#ifndef SYSEX_h
#define SYSEX_h

#include <Arduino.h>
#include <EEPROM.h>

#include "definitions.h"
#include "storage.h"
#include "functions.h"

void processSysex(unsigned char *, unsigned int);
void setKnobAsCC(byte, byte, byte);
void setKnobAsCCWithChannel(byte, byte, byte, byte);
void setKnobAsDisabled(byte);
void setKnobAsNRPN(byte, byte, byte);
void useHighResolution(bool);
void handleChangeChannel(byte);
void handleProgramChange(byte, byte);

#endif