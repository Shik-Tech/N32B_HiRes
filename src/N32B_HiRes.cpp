/*
  N32B Hi Res Firmware v3.6.0
  MIT License

  Copyright (c) 2022 SHIK
*/

#include <Arduino.h>

#include "definitions.h"
#include "functions.h"
#include "sysex.h"

void setup()
{
  n32b_display.setBright(0);
  n32b_display.setDigitLimit(2);

  muxFactory.init(MUX_S0, MUX_S1, MUX_S2, MUX_S3);
  muxFactory.setSignalPin(0, MUX_A_SIG);
  muxFactory.setSignalPin(1, MUX_B_SIG);

  /* Pin setup */
  pinMode(MIDI_TX_PIN, OUTPUT);
  digitalWrite(MIDI_TX_PIN, HIGH); // Prevent random messages on startup
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(MUX_A_SIG, INPUT);
  pinMode(MUX_B_SIG, INPUT);

  // Set debounce time to 50 milliseconds
  buttonA.setDebounceTime(50);
  buttonB.setDebounceTime(50);

  /*
  * Factory Reset
  * Hold button-A down while powering the device will reset the presets
  */
  if (!digitalRead(BUTTON_A_PIN))
  {
    bool buttonPressed = true;
    digitalWrite(LED_PIN, LOW);

    while (millis() < reset_timeout) // Check if button has been released before timeout
    {
      if (digitalRead(BUTTON_A_PIN))
      {
        buttonPressed = false;
        break;
      }
    }

    // If button is still held down, then clear eeprom
    if (buttonPressed)
    {
      // Blink once if reset request has been accepted
      digitalWrite(LED_PIN, HIGH);
      delay(20);
      digitalWrite(LED_PIN, LOW);

      // Clean eeprom
      for (unsigned int i = 0; i < EEPROM.length(); i++)
      {
        EEPROM.write(i, 0);
      }
      digitalWrite(LED_PIN, HIGH);
    }
  }

  // Write the factory presets to memory if the device was turn on for the first time
  if (!isEEPROMvalid())
  {
    for (int i = 0; i < NUMBER_OF_PRESETS; i++)
    {
      digitalWrite(LED_PIN, HIGH);
      delay(300);
      digitalWrite(LED_PIN, LOW);
      delay(300);
    }
    n32b_display.factoryResetAnimation();
    formatFactory();
  }

  // Load the last used preset as stored in EEPROM
  loadPreset(EEPROM.read(lastUsedPresetAddress));

  /* Set callbacks */
  MIDICoreUSB.setHandleMessage(onUsbMessage);
  MIDICoreSerial.setHandleMessage(onSerialMessage);

  MIDICoreUSB.setHandleSystemExclusive(processSysex);
  // MIDICoreSerial.setHandleSystemExclusive(processSysex);

  MIDICoreUSB.setHandleProgramChange(handleProgramChange);
  MIDICoreSerial.setHandleProgramChange(handleProgramChange);

  /* Initiate MIDI communications, listen to all channels */
  MIDICoreUSB.begin(MIDI_CHANNEL_OMNI);
  MIDICoreSerial.begin(MIDI_CHANNEL_OMNI);

  MIDICoreUSB.turnThruOn();
  MIDICoreSerial.turnThruOn();

  // Send an Active Sensing MIDI message to notify the target that the controller is on the bus
  MIDICoreUSB.sendRealTime((midi::MidiType)0xFE);
  MIDICoreSerial.sendRealTime((midi::MidiType)0xFE);

  n32b_display.showStartUpAnimation();
}

void loop()
{
  for (uint8_t currentKnob = 0; currentKnob < NUMBER_OF_KNOBS; currentKnob++)
  {
    muxFactory.update(currentKnob);
    updateKnob(currentKnob, disableKnobs);
  }
  doMidiRead();

  renderButtonFunctions();
  n32b_display.clearDisplay(disableKnobs);
}
