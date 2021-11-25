/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#ifndef FUNCTIONS_h
#define FUNCTIONS_h

#include <Arduino.h>
#include <USB-MIDI.h>

#include "definitions.h"
#include "storage.h"
// #include "variables.h"

USING_NAMESPACE_MIDI;

// void onUsbMessage(const MidiMessage& message)
void onUsbMessage(const midi::Message<128> &);

// void onSerialMessage(const MidiMessage& message)
void onSerialMessage(const midi::Message<128> &);

void interpretKnob(uint8_t, bool, bool, uint16_t);

void sendCCMessage(uint8_t, uint8_t, uint16_t, uint8_t);

void sendNRPM(uint8_t, uint8_t, int16_t, uint8_t);

// Handles the "menu" system, what to do when the button is pressed
void changeChannel(bool);

void changePreset(bool);

void buttonReleaseAction(bool);

void buttonPressAction(bool);

void renderButtonFunctions();

void doMidiRead();

uint16_t getKnobValue(uint8_t);

#endif