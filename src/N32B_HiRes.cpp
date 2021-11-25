/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

/* Libraries */
#include <Arduino.h>

#include "definitions.h"
#include "functions.h"
#include "sysex.h"

void setup()
{
  n32b_display.setIntensity(0); // 0 = min, 15 = max
  n32b_display.setScanLimit(1); // 0-7: Show 1-8 digits. Beware of currenct restrictions for 1-3 digits! See datasheet.
  n32b_display.setDecode(0xFF); // Enable "BCD Type B" decoding for all digits.
  n32b_display.setEnabled(true);
  n32b_display.setDigit(0, 15);
  n32b_display.setDigit(1, 15);
  n32b_display.display();

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
    for (int i = 0; i < 5; i++)
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

  // We don't want any incorrect data sent at startup so we fill the buffers
  // for (uint8_t i = 0; i < NUMBER_OF_KNOBS; i++)
  // {
  //   updateKnob(i);                 // Update the buffers
  //   interpretKnob(i, false, true); // Fill the emission buffers but do not send midi data
  // }

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

  /* Show factory reset animation */
  // n32b_display.showStartUpAnimation();
  // n32b_display.showChannelNumber(activePreset.channel);
}

void loop()
{
  for (uint8_t currentKnob = 0; currentKnob < NUMBER_OF_KNOBS; currentKnob++)
  {
    muxFactory.setMultiplexer(currentKnob);
    interpretKnob(currentKnob, false, inhibitMidi, muxFactory.readSingle(currentKnob));
  }
  doMidiRead();

  renderButtonFunctions(); // Update buttons stats
  n32b_display.updateDisplay();
}
