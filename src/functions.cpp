/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#include "functions.h"

void onUsbMessage(const midi::Message<128> &message)
{
  MIDICoreSerial.send(message);
  n32b_display.blinkDot(2);
}

void onSerialMessage(const midi::Message<128> &message)
{
  // MIDICoreUSB.sendControlChange(message.data1, message.data2, message.channel);
  if (MIDICoreSerial.getType() != midi::MidiType::ActiveSensing)
  {
    MIDICoreUSB.send(message.type, message.data1, message.data2, message.channel);
    n32b_display.blinkDot(2);
  }
}

void updateKnob(uint8_t index, bool inhibit)
{
  if (knobValues[index][0] != knobValues[index][1])
  {
    uint16_t shiftedValue = map(knobValues[index][0], 0, 1021, 0, 16383);
    uint8_t MSBValue = shiftedValue >> 7;
    uint8_t LSBValue = lowByte(shiftedValue) >> 1;

    if (!inhibit)
    {

      if (activePreset.knobInfo[index].NRPN == 0)
      {
        uint8_t knobChannel = activePreset.knobInfo[index].CHANNEL & 0x7f;
        if (knobChannel > 0 && knobChannel < 17)
        {
          sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, MSBValue, LSBValue, knobChannel);
        }
        else if (knobChannel == 0)
        {
          sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, MSBValue, LSBValue, activePreset.channel);
        }
      }
      else
      {
        sendNRPM(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, MSBValue, LSBValue, activePreset.channel);
      }
    }

    knobValues[index][1] = knobValues[index][0];
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

void changeChannel(bool direction)
{
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
    // Previous Channel
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
  direction ? isPressingAButton = false : isPressingBButton = false;

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
  if (
      (isPressingAButton || isPressingBButton) &&
      (millis() - pressedTime > (unsigned int)(SHORT_PRESS_TIME << 2)))
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
