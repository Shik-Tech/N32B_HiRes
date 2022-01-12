/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#include "functions.h"

// void onUsbMessage(const MidiMessage& message)
void onUsbMessage(const midi::Message<128> &message)
{
  MIDICoreSerial.send(message);
  n32b_display.blinkDot(0);
}

// void onSerialMessage(const MidiMessage& message)
void onSerialMessage(const midi::Message<128> &message)
{
  // MIDICoreUSB.sendControlChange(message.data1, message.data2, message.channel);
  if (MIDICoreSerial.getType() != midi::MidiType::ActiveSensing)
  {
    MIDICoreUSB.send(message.type, message.data1, message.data2, message.channel);
    n32b_display.blinkDot(0);
  }
}

void updateKnob(uint8_t index, bool inhibit)
{
  uint8_t readMSBValue = getKnobValue(index, false);
  uint8_t readLSBValue = getKnobValue(index, true);
  bool isMoving = true;
  for (uint8_t i = 0; i < 8; i++)
  {
    isMoving = isMoving & readLSBValue != emittedValue[index][i];
  }
  if (isMoving && !inhibit)
  {
    if (activePreset.knobInfo[index].NRPN == 0)
    {
      uint8_t knobChannel = activePreset.knobInfo[index].CHANNEL & 0x7f;
      if (knobChannel > 0 && knobChannel < 17)
      {
        sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, readMSBValue, readLSBValue, knobChannel);
      }
      else if (knobChannel == 0)
      {
        sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, readMSBValue, readLSBValue, activePreset.channel);
      }
    }
    else
    {
      sendNRPM(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, readMSBValue, readLSBValue, activePreset.channel);
    }
    for (uint8_t i = 7; i > 0; i--)
    {
      emittedValue[index][i] = emittedValue[index][i - 1];
    }
    emittedValue[index][0] = readLSBValue;
  }
}

void sendCCMessage(uint8_t MSB, uint8_t LSB, uint8_t MSBvalue, uint8_t LSBvalue, uint8_t channel)
{
  if (activePreset.highResolution)
  {
    MIDICoreSerial.sendControlChange(MSB, MSBvalue, channel);
    MIDICoreSerial.sendControlChange(LSB, LSBvalue, channel);

    MIDICoreUSB.sendControlChange(MSB, MSBvalue, channel);
    MIDICoreUSB.sendControlChange(LSB, LSBvalue, channel);
    n32b_display.showValue(MSBvalue);
  }
  else
  {
    MIDICoreSerial.sendControlChange(MSB, MSBvalue, channel);
    MIDICoreUSB.sendControlChange(MSB, MSBvalue, channel);
    n32b_display.showValue(MSBvalue);
  }
  // n32b_display.blinkDot(1);
}

void sendNRPM(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, uint8_t MSBvalue, uint8_t LSBvalue, uint8_t channel)
{
  // uint16_t shiftedValue = map(value, 0, 1023, 0, 16383) << 1;
  // uint16_t shiftedValue = map(value, 0, 1023, 0, 16383);

  MIDICoreSerial.sendControlChange(99, NRPNNumberMSB & 0x7F, channel); // NRPN MSB
  MIDICoreUSB.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);    // NRPN MSB

  MIDICoreSerial.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); // NRPN LSB
  MIDICoreUSB.sendControlChange(98, NRPNNumberLSB & 0x7F, channel);    // NRPN LSB

  MIDICoreSerial.sendControlChange(6, MSBvalue, channel); // Data Entry MSB
  MIDICoreUSB.sendControlChange(6, MSBvalue, channel);    // Data Entry MSB

  if (activePreset.highResolution)
  {
    MIDICoreSerial.sendControlChange(38, MSBvalue, channel); // LSB for Control 6 (Data Entry)
    MIDICoreUSB.sendControlChange(38, MSBvalue, channel);    // LSB for Control 6 (Data Entry)
  }
  n32b_display.blinkDot(1);
}

// Handles the "menu" system, what to do when the button is pressed
void changeChannel(bool direction)
{
  // inhibitMidi = true;
  if (direction)
  {
    // Next Channel
    if (activePreset.channel < 16)
      activePreset.channel++;
    else
      activePreset.channel = 1;
  }
  else
  {
    if (activePreset.channel > 1)
      activePreset.channel--;
    else
      activePreset.channel = 16;
  }
}

void changePreset(bool direction)
{
  if (direction)
  {
    // Next Preset
    if (currentPresetNumber < 4)
      loadPreset(currentPresetNumber + 1);
    else
      loadPreset(0);
  }
  else
  {
    // Previous Preset
    if (currentPresetNumber > 0)
      loadPreset(currentPresetNumber - 1);
    else
      loadPreset(4);
  }
  // MIDICoreSerial.sendProgramChange(currentPresetNumber, 1);
  // MIDICoreUSB.sendProgramChange(currentPresetNumber, 1);
}

void buttonReleaseAction(bool direction)
{
  if (direction)
  {
    isPressingAButton = false;
  }
  else
  {
    isPressingBButton = false;
  }

  if (millis() - pressedTime < SHORT_PRESS_TIME)
  {
    if (isPresetMode)
    {
      changePreset(direction);
      n32b_display.showPresetNumber(currentPresetNumber);
    }
    else
    {
      changeChannel(direction);
      n32b_display.showChannelNumber(activePreset.channel);
    }
  }

  MIDICoreUSB.turnThruOn();
  MIDICoreSerial.turnThruOn();
}

void buttonPressAction(bool direction)
{
  pressedTime = millis();
  // inhibitMidi = true;

  MIDICoreSerial.turnThruOff();
  MIDICoreUSB.turnThruOff();
}

void renderButtonFunctions()
{
  // Must call the loop() function first
  buttonA.loop();
  buttonB.loop();

  if (buttonA.isPressed())
  {
    isPressingAButton = true;
    buttonPressAction(1);
  }

  if (buttonB.isPressed())
  {
    isPressingBButton = true;
    buttonPressAction(0);
  }

  if (buttonA.isReleased())
  {
    buttonReleaseAction(1);
  }

  if (buttonB.isReleased())
  {
    buttonReleaseAction(0);
  }

  // Switch between channelMode and presetMode
  if ((isPressingAButton || isPressingBButton) && (millis() - pressedTime > LONG_PRESS_TIME))
  {
    if (isPressingAButton)
    {
      isPresetMode = false;
      n32b_display.showChannelNumber(activePreset.channel);
    }
    if (isPressingBButton)
    {
      isPresetMode = true;
      n32b_display.showPresetNumber(currentPresetNumber);
    }
  }
}

void doMidiRead()
{
  MIDICoreSerial.read();
  MIDICoreUSB.read();
}

uint8_t getKnobValue(uint8_t index, bool isLSB)
{
  uint16_t average = 0;
  for (uint8_t i = 0; i < 4; i++)
  {
    average += knobBuffer[index][isLSB][i];
  }
  average /= 4;
  return average;
}