/*
  N32B Hi Res Firmware v3.6.0
  MIT License

  Copyright (c) 2022 SHIK
*/

/*
 * Definitions of constats
 */

#ifndef N32B_DEFINITIONS
#define N32B_DEFINITIONS

#include <Arduino.h>
#include <USB-MIDI.h>
#include <ezButton.h>

#include "mux_factory.h"
#include "display.h"

USING_NAMESPACE_MIDI;

const uint8_t firmwareVersion[] PROGMEM = {3, 6, 0};

extern MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport> MIDICoreUSB;
extern MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>> MIDICoreSerial;
extern MUX_FACTORY muxFactory;
extern N32B_DISPLAY n32b_display;
extern ezButton buttonA;
extern ezButton buttonB;

/* Pin setup */
enum PINS
{
  MUX_A_SIG = 8,
  MUX_B_SIG = 9,
  MIDI_TX_PIN = 1,
  MUX_S0 = 2,
  MUX_S1 = 3,
  MUX_S2 = 4,
  MUX_S3 = 5,
  LED_PIN = 17,
  DIN = 16,
  CS = 10,
  CLK = 15,
  BUTTON_A_PIN = A3,
  BUTTON_B_PIN = A2
};

// Reset to factory preset timeout
extern const unsigned int reset_timeout;

// SYSEX constants
extern const uint8_t SHIK_MANUFACTURER_ID;

enum COMMANDS_INDEXS
{
  MANUFACTURER = 1,
  COMMAND = 2,
  KNOB_INDEX = 3,
  MSB_INDEX = 4,
  LSB_INDEX = 5,
  CHANNEL_INDEX = 6,
  MODE_INDEX = 7,
  INVERT_A_INDEX = 8,
  INVERT_B_INDEX = 9
};

enum COMMANDS
{
  SET_KNOB_MODE = 1,       // Define knob mode (see KNOB_MODES)
  SAVE_PRESET = 2,         // Save the preset
  LOAD_PRESET = 3,         // Load a preset
  SEND_CURRENT_CONFIG = 4, // Send the current config
  SYNC_KNOBS = 5,          // Forces the emission of the messages associated to every knob
  CHANGE_CHANNEL = 6       // Changes the global MIDI channel
};

enum KNOB_MODES
{
  KNOB_MODE_DISABLE = 0,
  KNOB_MODE_STANDARD = 1,
  KNOB_MODE_DUAL = 2,
  KNOB_MODE_NRPN = 3,
  KNOB_MODE_RPN = 4,
  KNOB_MODE_HIRES = 5
};

// General definitions
extern const uint8_t NUMBER_OF_KNOBS;
extern const uint8_t NUMBER_OF_PRESETS;

// Knob settings structure
struct Knob_t
{
  uint8_t MSB;
  uint8_t LSB;
  midi::Channel CHANNEL;
  uint8_t MODE;
  bool INVERT_A;
  bool INVERT_B;
};

// A preset struct is defining the device preset structure
struct Preset_t
{
  midi::Channel channel;
  Knob_t knobInfo[32];
};

/* Device setup data */
extern byte currentPresetNumber;
extern Preset_t activePreset;
extern uint16_t knobValues[32][4];
extern float EMA_a; // EMA alpha
extern bool disableKnobs;

/* Buttons variables */
extern const uint8_t SHORT_PRESS_TIME; // Milliseconds
extern unsigned long pressedTime;
extern bool isPressingAButton;
extern bool isPressingBButton;

/* Mode variables */
extern bool isPresetMode;

// byte index in EEPROM for the last used preset
extern uint8_t lastUsedPresetAddress;

#endif