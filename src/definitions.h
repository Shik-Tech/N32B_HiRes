/*
  N32B Hi Res Firmware v3.5.2
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

extern MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport> MIDICoreUSB;
extern MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<HardwareSerial>> MIDICoreSerial;
extern MUX_FACTORY muxFactory;
extern N32B_DISPLAY n32b_display;
extern ezButton buttonA;
extern ezButton buttonB;

/* Pin setup */
extern const uint8_t MUX_A_SIG;
extern const uint8_t MUX_B_SIG;
extern const uint8_t MIDI_TX_PIN;
extern const uint8_t MUX_S0;
extern const uint8_t MUX_S1;
extern const uint8_t MUX_S2;
extern const uint8_t MUX_S3;
extern const uint8_t LED_PIN;
extern const uint8_t BUTTON_A_PIN;
extern const uint8_t BUTTON_B_PIN;

// Reset to factory preset timeout
extern const unsigned int reset_timeout;

/*--- EEPROM Format Chuncks ---*/
// Change these any time the data structure of a preset changed
// This will trigger reformatting on the next startup
extern const uint8_t MAJOR_VERSION;
extern const uint8_t MINOR_VERSION;
extern const uint8_t POINT_VERSION;

// SYSEX constants
extern const uint8_t SHIK_MANUFACTURER_ID;

extern const uint8_t MANUFACTURER;
extern const uint8_t COMMAND;
extern const uint8_t KNOB_INDEX;
extern const uint8_t MSB_INDEX;
extern const uint8_t LSB_INDEX;
extern const uint8_t CHANNEL_INDEX;

extern const uint8_t SET_KNOB_AS_CC;         // CC
extern const uint8_t SET_KNOB_AS_CC_CHANNEL; // CC & Channel
extern const uint8_t SET_KNOB_AS_NRPN;       // NRPN
extern const uint8_t SAVE_PRESET;            // Save the preset
extern const uint8_t LOAD_PRESET;            // Load a preset
extern const uint8_t SEND_CURRENT_CONFIG;    // Send the current config
extern const uint8_t SYNC_KNOBS;             // Forces the emission of the messages associated to every knob
extern const uint8_t CHANGE_CHANNEL;         // Changes the global MIDI channel
extern const uint8_t DISABLE_KNOB;           // Disable
extern const uint8_t HIGH_RES_14BIT;         // Use 7-bit or 14-bit midi messages

// General definitions
extern const uint8_t NUMBER_OF_KNOBS;
extern const uint8_t NUMBER_OF_PRESETS;

// A preset struct is defining the device preset structure
struct Preset_t
{

  // channel the device is sending on
  midi::Channel channel;

  // Knob settings structure
  struct Knob_t
  {
    uint8_t MSB;
    uint8_t LSB;
    midi::Channel CHANNEL;
    bool NRPN;
    // bool RPN;
  } knobInfo[32];

  // High resolution 14-bit
  bool highResolution;
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