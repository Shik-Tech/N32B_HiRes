/*
  N32B Hi Res Firmware v3.5.2
  MIT License

  Copyright (c) 2022 SHIK
*/

#include "sysex.h"

void processSysex(unsigned char *data, unsigned int size)
{
    if (size > 3 && data[MANUFACTURER] == SHIK_MANUFACTURER_ID)
    {
        if (data[COMMAND] == SET_KNOB_AS_CC)
        {
            setKnobAsCC(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX]);
        }
        if (data[COMMAND] == SET_KNOB_AS_CC_CHANNEL)
        {
            setKnobAsCCWithChannel(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX], data[CHANNEL_INDEX]);
        }
        if (data[COMMAND] == SET_KNOB_AS_NRPN)
        {
            setKnobAsNRPN(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX]);
        }
        if (data[COMMAND] == DISABLE_KNOB)
        {
            setKnobAsDisabled(data[KNOB_INDEX]);
        }
        if (data[COMMAND] == HIGH_RES_14BIT)
        {
            useHighResolution(data[KNOB_INDEX], data[KNOB_INDEX]);
        }
        if (data[COMMAND] == SAVE_PRESET)
        {
            savePreset(data[KNOB_INDEX]);
        }
        if (data[COMMAND] == LOAD_PRESET)
        {
            loadPreset(data[KNOB_INDEX]);
        }
        // if (data[COMMAND] == SYNC_KNOBS)
        // {
        //     for (uint8_t i = 0; i < NUMBER_OF_KNOBS; i++)
        //     {
        //         interpretKnob(i, true, false);
        //     }
        // }
        if (data[COMMAND] == CHANGE_CHANNEL)
        {
            handleChangeChannel(data[KNOB_INDEX]);
        }
        if (data[COMMAND] == SEND_CURRENT_CONFIG)
        {
            // sendCurrentConfig();
        }
    }
    else
    {
        // Serial.println("Manufacturer ID is not matching the N32B");
    }
}

void setKnobAsCC(byte knobIndex, byte MSB, byte LSB)
{
    if (knobIndex < NUMBER_OF_KNOBS)
    {
        activePreset.knobInfo[knobIndex].MSB = MSB;
        activePreset.knobInfo[knobIndex].LSB = LSB;
        activePreset.knobInfo[knobIndex].MODE = 0;
        activePreset.knobInfo[knobIndex].CHANNEL = 128;
    }
}

void setKnobAsCCWithChannel(byte knobIndex, byte MSB, byte LSB, byte channel)
{
    if (knobIndex < NUMBER_OF_KNOBS)
    {
        activePreset.knobInfo[knobIndex].MSB = MSB;
        activePreset.knobInfo[knobIndex].LSB = LSB;
        activePreset.knobInfo[knobIndex].MODE = 0;
        activePreset.knobInfo[knobIndex].CHANNEL = channel | 0x80;
    }
}

void setKnobAsDisabled(byte knobIndex)
{
    if (knobIndex < NUMBER_OF_KNOBS)
    {
        activePreset.knobInfo[knobIndex].MSB = 0;
        activePreset.knobInfo[knobIndex].LSB = 0;
        activePreset.knobInfo[knobIndex].MODE = 0;
        activePreset.knobInfo[knobIndex].CHANNEL = 17 | 0x80; // Make the knob out of range to disable it
    }
}

void setKnobAsNRPN(byte knobIndex, byte LSB, byte MSB)
{
    if (knobIndex < NUMBER_OF_KNOBS)
    {
        activePreset.knobInfo[knobIndex].MSB = MSB;
        activePreset.knobInfo[knobIndex].LSB = LSB;
        activePreset.knobInfo[knobIndex].MODE = 2;
        activePreset.knobInfo[knobIndex].CHANNEL = 128;
    }
}

void useHighResolution(byte knobIndex, bool value)
{
    activePreset.knobInfo[knobIndex].highResolution = value;
}

void handleChangeChannel(byte channel)
{
    if (channel < 17 && channel > 0)
    {
        activePreset.channel = channel;
    }
}

// Change preset on program change
void handleProgramChange(byte channel, byte number)
{
    if (number < 5)
    {
        loadPreset(number);
    }
}