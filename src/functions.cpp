/*
  N32B Hi Res Firmware v3.6.0
  MIT License

  Copyright (c) 2022 SHIK
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
  Knob_t &currentKnob = activePreset.knobInfo[index];
  bool needToUpdate = false;
  uint16_t shiftedValue;
  uint8_t MSBValue;
  uint8_t LSBValue;
  if (currentKnob.MODE == KNOB_MODE_HIRES)
  {
    if (
        (knobValues[index][0] != knobValues[index][1]) &&
        (knobValues[index][0] != knobValues[index][2]) &&
        (knobValues[index][0] != knobValues[index][3]))
    {
      needToUpdate = true;
      shiftedValue = map(knobValues[index][0], 0, 1019, 0, 16383);
      MSBValue = shiftedValue >> 7;
      LSBValue = lowByte(shiftedValue) >> 1;
    }
  }
  else
  {
    shiftedValue = map(knobValues[index][0], 0, 1019, 0, 16383);
    MSBValue = shiftedValue >> 7;

    uint8_t BufferValue1 = map(knobValues[index][1], 0, 1019, 0, 16383) >> 7;
    uint8_t BufferValue2 = map(knobValues[index][2], 0, 1019, 0, 16383) >> 7;
    uint8_t BufferValue3 = map(knobValues[index][3], 0, 1019, 0, 16383) >> 7;
    if (
        (MSBValue != BufferValue1) &&
        (MSBValue != BufferValue2) &&
        (MSBValue != BufferValue3))
    {
      needToUpdate = true;
    }
  }

  if (needToUpdate)
  {
    midi::Channel channel = currentKnob.CHANNEL > 0 && currentKnob.CHANNEL < 17 ? currentKnob.CHANNEL : activePreset.channel;

    if (!inhibit)
    {
      switch (currentKnob.MODE)
      {
      case KNOB_MODE_STANDARD:
      case KNOB_MODE_HIRES:
        sendCCMessage(currentKnob, MSBValue, LSBValue, channel);
        break;

      case KNOB_MODE_DUAL:
        sendDualCCMessage(currentKnob, MSBValue, channel);
        break;

      case KNOB_MODE_NRPN:
        sendNRPM(currentKnob, MSBValue, LSBValue, channel);
        break;

      case KNOB_MODE_RPN:
        sendRPM(currentKnob, MSBValue, LSBValue, channel);
        break;

      default:
        break;
      }
    }

    knobValues[index][3] = knobValues[index][2];
    knobValues[index][2] = knobValues[index][1];
    knobValues[index][1] = knobValues[index][0];
  }
}

void sendCCMessage(const struct Knob_t &currentKnob, uint8_t MSBvalue, uint8_t LSBvalue, midi::Channel channel)
{
  uint8_t MSBSendValue = currentKnob.INVERT_A ? 127 - MSBvalue : MSBvalue;
  uint8_t LSBSendValue = currentKnob.INVERT_A ? 127 - LSBvalue : LSBvalue;
  if (currentKnob.MODE == KNOB_MODE_HIRES)
  {
    MIDICoreSerial.sendControlChange(currentKnob.MSB, MSBSendValue, channel);
    MIDICoreSerial.sendControlChange(currentKnob.LSB, LSBSendValue, channel);

    MIDICoreUSB.sendControlChange(currentKnob.MSB, MSBSendValue, channel);
    MIDICoreUSB.sendControlChange(currentKnob.LSB, LSBSendValue, channel);
  }
  else
  {
    MIDICoreSerial.sendControlChange(currentKnob.MSB, MSBSendValue, channel);
    MIDICoreUSB.sendControlChange(currentKnob.MSB, MSBSendValue, channel);
  }
  n32b_display.blinkDot(1);
}

void sendDualCCMessage(const struct Knob_t &currentKnob, uint8_t MSBvalue, midi::Channel channel)
{
  uint8_t MSBSendValue = currentKnob.INVERT_A ? 127 - MSBvalue : MSBvalue;
  uint8_t LSBSendValue = currentKnob.INVERT_B ? 127 - MSBvalue : MSBvalue;
  MIDICoreSerial.sendControlChange(currentKnob.MSB, MSBSendValue, channel);
  MIDICoreSerial.sendControlChange(currentKnob.LSB, LSBSendValue, channel);

  MIDICoreUSB.sendControlChange(currentKnob.MSB, MSBSendValue, channel);
  MIDICoreUSB.sendControlChange(currentKnob.LSB, LSBSendValue, channel);

  n32b_display.blinkDot(1);
}

void sendNRPM(const struct Knob_t &currentKnob, uint8_t MSBvalue, uint8_t LSBvalue, midi::Channel channel)
{
  MIDICoreSerial.sendControlChange(99, currentKnob.MSB & 0x7F, channel); // NRPN MSB
  MIDICoreUSB.sendControlChange(99, currentKnob.MSB & 0x7F, channel);    // NRPN MSB

  MIDICoreSerial.sendControlChange(98, currentKnob.LSB & 0x7F, channel); // NRPN LSB
  MIDICoreUSB.sendControlChange(98, currentKnob.LSB & 0x7F, channel);    // NRPN LSB

  MIDICoreSerial.sendControlChange(6, MSBvalue, channel); // Data Entry MSB
  MIDICoreUSB.sendControlChange(6, MSBvalue, channel);    // Data Entry MSB

  n32b_display.blinkDot(1);
}

void sendRPM(const struct Knob_t &currentKnob, uint8_t MSBvalue, uint8_t LSBvalue, midi::Channel channel)
{
  MIDICoreSerial.sendControlChange(101, currentKnob.MSB & 0x7F, channel); // RPN MSB
  MIDICoreUSB.sendControlChange(101, currentKnob.MSB & 0x7F, channel);    // RPN MSB

  MIDICoreSerial.sendControlChange(100, currentKnob.LSB & 0x7F, channel); // RPN LSB
  MIDICoreUSB.sendControlChange(100, currentKnob.LSB & 0x7F, channel);    // RPN LSB

  MIDICoreSerial.sendControlChange(6, MSBvalue, channel); // Data Entry MSB
  MIDICoreUSB.sendControlChange(6, MSBvalue, channel);    // Data Entry MSB

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
    if (currentPresetNumber < NUMBER_OF_PRESETS - 1)
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
      loadPreset(NUMBER_OF_PRESETS - 1);
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
      n32b_display.showPresetNumber(currentPresetNumber, disableKnobs);
    }
    else
    {
      changeChannel(direction);
      n32b_display.showChannelNumber(activePreset.channel, disableKnobs);
    }
  }

  MIDICoreUSB.turnThruOn();
  MIDICoreSerial.turnThruOn();
}

void buttonPressAction(bool direction)
{
  pressedTime = millis();
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
      n32b_display.showChannelNumber(activePreset.channel, disableKnobs);
    }
    if (isPressingBButton)
    {
      isPresetMode = true;
      n32b_display.showPresetNumber(currentPresetNumber, disableKnobs);
    }
  }
}

void doMidiRead()
{
  MIDICoreSerial.read();
  MIDICoreUSB.read();
}
