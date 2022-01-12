/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#include "definitions.h"

USBMIDI_CREATE_INSTANCE(0, MIDICoreUSB);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDICoreSerial);

MUX_FACTORY muxFactory;

N32B_DISPLAY n32b_display(1, 10);

ezButton buttonA(BUTTON_A_PIN);
ezButton buttonB(BUTTON_B_PIN);

/* Pin setup */
const uint8_t MUX_A_SIG = 8;
const uint8_t MUX_B_SIG = 9;
const uint8_t MIDI_TX_PIN = 1;
const uint8_t MUX_S0 = 2;
const uint8_t MUX_S1 = 3;
const uint8_t MUX_S2 = 4;
const uint8_t MUX_S3 = 5;
const uint8_t LED_PIN = 17;
const uint8_t BUTTON_A_PIN = A3;
const uint8_t BUTTON_B_PIN = A2;

// Reset to factory preset timeout
const unsigned int reset_timeout = 4000;

/*--- EEPROM Format Chuncks ---*/
// Change these any time the data structure of a preset changed
// This will trigger reformatting on the next startup
const uint8_t MAJOR_VERSION = 3;
const uint8_t MINOR_VERSION = 0;
const uint8_t POINT_VERSION = 2;

// SYSEX constants
const uint8_t SHIK_MANUFACTURER_ID = 32;

const uint8_t MANUFACTURER = 1;
const uint8_t COMMAND = 2;
const uint8_t KNOB_INDEX = 3;
const uint8_t MSB_INDEX = 4;
const uint8_t LSB_INDEX = 5;
const uint8_t CHANNEL_INDEX = 6;

const uint8_t SET_KNOB_AS_CC = 1;         // CC
const uint8_t SET_KNOB_AS_CC_CHANNEL = 2; // CC & Channel
const uint8_t SET_KNOB_AS_NRPN = 3;       // NRPN
const uint8_t SAVE_PRESET = 5;            // Save the preset
const uint8_t LOAD_PRESET = 6;            // Load a preset
const uint8_t SEND_CURRENT_CONFIG = 7;    // Send the current config
const uint8_t SYNC_KNOBS = 8;             // Forces the emission of the messages associated to every knob
const uint8_t CHANGE_CHANNEL = 9;         // Changes the global MIDI channel
const uint8_t DISABLE_KNOB = 11;          // Disable
const uint8_t HIGH_RES_14BIT = 14;        // Use 7-bit or 14-bit midi messages

// General definitions
const uint8_t NUMBER_OF_KNOBS = 32;
const uint8_t NUMBER_OF_PRESETS = 5;

/* Device setup data */
byte currentPresetNumber = 0;
Preset_t activePreset;

uint8_t knobBuffer[32][2][3] = {0};
uint8_t emittedValue[32][9] = {0};

/* Buttons variables */
const uint8_t SHORT_PRESS_TIME = 255; // Milliseconds
unsigned long pressedTime = 0;
bool isPressingAButton = false;
bool isPressingBButton = false;

/* Mode variables */
bool startUp = true;
bool isPresetMode = false;
bool inhibitMidi = false;

bool wasFactoryReset = false;
// byte index in EEPROM for the last used preset
uint8_t lastUsedPresetAddress = 0;
