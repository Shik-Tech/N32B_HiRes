/*
  N32B Hi Res Firmware v3.6.0
  MIT License

  Copyright (c) 2022 SHIK
*/

#include "definitions.h"

USBMIDI_CREATE_INSTANCE(0, MIDICoreUSB);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDICoreSerial);

MUX_FACTORY muxFactory;
N32B_DISPLAY n32b_display(DIN, CS, CLK);

ezButton buttonA(BUTTON_A_PIN);
ezButton buttonB(BUTTON_B_PIN);

/* Device setup data */
byte currentPresetNumber = 0;
Preset_t activePreset;

// uint16_t bufferKnobValues[32][3];
uint16_t knobValues[32][4] = {0};
float EMA_a = 0.2;

/* Buttons variables */
const unsigned int reset_timeout = 4000; // Reset to factory preset timeout
const uint8_t SHORT_PRESS_TIME = 255;    // Milliseconds
unsigned long pressedTime = 0;
bool isPressingAButton = false;
bool isPressingBButton = false;

/* Mode variables */
bool isPresetMode = false;

// byte index in EEPROM for the last used preset
uint8_t lastUsedPresetAddress = 0;
