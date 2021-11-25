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
  if (MIDICoreSerial.getType() < 254)
  {
    MIDICoreUSB.send(message.type, message.data1, message.data2, message.channel);
    n32b_display.blinkDot(0);
  }
}

int potCState[32] = {0};       // Current state of the pot
int potPState[32] = {0};       // Previous state of the pot
int midiCState[32] = {0};      // Current state of the midi value
int midiPState[32] = {0};      // Previous state of the midi value
int varThreshold = 6;          //* Threshold for the potentiometer signal variation
unsigned long PTime[32] = {0}; // Previously stored time
unsigned long timer[32] = {0}; // Stores the time that has elapsed since the timer was reset
unsigned int TIMEOUT = 300;    //* Amount of time the potentiometer will be read after it exceeds the varThreshold
boolean potMoving = true;      // If the potentiometer is moving

void interpretKnob(uint8_t index, bool force, bool inhibit, uint16_t readValue)
{
  potCState[index] = readValue;
  midiCState[index] = readValue;
  // midiCState[index] = map(potCState[index], 0, 1023, 0, 127); // Maps the reading of the potCState to a value usable in midi
  
  // Calculates the absolute value between the difference between the current and previous state of the pot
  int potVar = abs(potCState[index] - potPState[index]);

  if (potVar > varThreshold)
  {                          // Opens the gate if the potentiometer variation is greater than the threshold
    PTime[index] = millis(); // Stores the previous time
  }

  timer[index] = millis() - PTime[index]; // Resets the timer 11000 - 11000 = 0ms

  if (timer[index] < TIMEOUT)
  { // If the timer is less than the maximum allowed time it means that the potentiometer is still moving
    potMoving = true;
  }
  else
  {
    potMoving = false;
  }

  if (potMoving == true)
  { // If the potentiometer is still moving, send the change control
    if (midiPState[index] != midiCState[index])
    {
      //Serial.println(midiCState);
      uint8_t knobChannel = activePreset.knobInfo[index].CHANNEL & 0x7f;
      if (knobChannel > 0 && knobChannel < 17)
      {
        sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, readValue, knobChannel);
      }
      else if (knobChannel == 0)
      {
        sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, readValue, activePreset.channel);
      }
      // n32b_display.blinkDot(1);

      potPState[index] = potCState[index]; // Stores the current reading of the potentiometer to compare with the next
      midiPState[index] = midiCState[index];
    }
  }

  //////////////////////////////////////
  // Read knob value
  // uint16_t toSend = getKnobValue(index);
  // If the value to send is relevant, send it to the MIDI OUT port
  // if (((toSend != emittedValue[0][index]) && (toSend != emittedValue[1][index]) && (toSend != emittedValue[2][index])) || (force == true))
  // uint16_t toSend = averageValues[index];
  // if (((toSend != emittedValue[0][index]) && (toSend != emittedValue[1][index]) && (toSend != emittedValue[2][index])) || (force == true))
  // {
  //   if (!inhibit)
  //   {
  //     if (index == 15)
  //     {
  //       // Serial.print("toSend: ");
  //       // Serial.println(toSend);
  //       // Serial.println(emittedValue[0][index]);
  //       // Serial.println(emittedValue[1][index]);
  //       // Serial.println(emittedValue[2][index]);
  //       // Serial.println("-------------------------");

  //       Serial.print("toSend: ");
  //       Serial.println(toSend);
  //       // Serial.println(knobBuffer[0][index]);
  //       Serial.println(knobBuffer[1][index]);
  //       Serial.println(knobBuffer[2][index]);
  //       Serial.println(knobBuffer[3][index]);
  //       Serial.println("-------------------------");
  //     }
  //     // CC or NRPN?
  //     if (activePreset.knobInfo[index].NRPN == 0)
  //     {
  //       // CC Message
  //       // Check if it is channel specific message
  //       uint8_t knobChannel = activePreset.knobInfo[index].CHANNEL & 0x7f;
  //       if (knobChannel > 0 && knobChannel < 17)
  //       {
  //         sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, knobChannel);
  //       }
  //       else if (knobChannel == 0)
  //       {
  //         sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, activePreset.channel);
  //       }
  //       n32b_display.blinkDot(1);
  //     }
  //     else
  //     {
  //       // NRPN Message
  //       sendNRPM(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, activePreset.channel);
  //       n32b_display.blinkDot(1);
  //     }
  //   }

  //   // Fill the emission buffers
  //   for (uint8_t i = 2; i > 0; i--)
  //   {
  //     emittedValue[i][index] = emittedValue[i - 1][index];
  //   }
  //   emittedValue[0][index] = toSend;
  // }
}

void sendCCMessage(uint8_t MSB, uint8_t LSB, uint16_t value, uint8_t channel)
{
  if (activePreset.highResolution)
  {
    unsigned int shiftedValue = map(value, 0, 1023, 0, 16383);
    MIDICoreSerial.sendControlChange(MSB, shiftedValue >> 7, channel);
    MIDICoreSerial.sendControlChange(LSB, lowByte(shiftedValue) >> 1, channel);

    MIDICoreUSB.sendControlChange(MSB, shiftedValue >> 7, channel);
    MIDICoreUSB.sendControlChange(LSB, lowByte(shiftedValue) >> 1, channel);
    n32b_display.showValue(shiftedValue >> 7);
  }
  else
  {
    // uint8_t lowResSend = map(value, 0, 1023, 0, 127);
    MIDICoreSerial.sendControlChange(MSB, value, channel);
    MIDICoreUSB.sendControlChange(MSB, value, channel);
    n32b_display.showValue(value);
  }
}

void sendNRPM(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, int16_t value, uint8_t channel)
{
  unsigned int shiftedValue = map(value, 0, 1023, 0, 16383) << 1;

  MIDICoreSerial.sendControlChange(99, NRPNNumberMSB & 0x7F, channel); // NRPN MSB
  MIDICoreUSB.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);    // NRPN MSB

  MIDICoreSerial.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); // NRPN LSB
  MIDICoreUSB.sendControlChange(98, NRPNNumberLSB & 0x7F, channel);    // NRPN LSB

  MIDICoreSerial.sendControlChange(6, highByte(shiftedValue), channel); // Data Entry MSB
  MIDICoreUSB.sendControlChange(6, highByte(shiftedValue), channel);    // Data Entry MSB

  if (activePreset.highResolution)
  {
    MIDICoreSerial.sendControlChange(38, lowByte(shiftedValue) >> 1, channel); // LSB for Control 6 (Data Entry)
    MIDICoreUSB.sendControlChange(38, lowByte(shiftedValue) >> 1, channel);    // LSB for Control 6 (Data Entry)
  }
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

// uint16_t getKnobValue(uint8_t index)
// {
//   uint16_t average = 0;
//   for (uint8_t i = 0; i < 4; i++)
//   {
//     average += knobBuffer[i][index];
//   }
//   average /= 4;

// #ifdef MK2
//   return 1023 - average;
// #endif

// #ifndef MK2
//   return average;
// #endif
// }