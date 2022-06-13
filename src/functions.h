/*
  N32B Hi Res Firmware v3.5.2
  MIT License

  Copyright (c) 2022 SHIK
*/

#ifndef FUNCTIONS_h
#define FUNCTIONS_h

#include <Arduino.h>
#include <USB-MIDI.h>

#include "definitions.h"
#include "storage.h"

USING_NAMESPACE_MIDI;

void onUsbMessage(const midi::Message<128> &);
void onSerialMessage(const midi::Message<128> &);

void updateKnob(uint8_t, bool);

void sendCCMessage(const struct Knob_t &, uint8_t, uint8_t);
void sendNRPM(const struct Knob_t &, uint8_t, uint8_t);

void changeChannel(bool);
void changePreset(bool);

void buttonReleaseAction(bool);
void buttonPressAction(bool);
void renderButtonFunctions();

void doMidiRead();

#endif