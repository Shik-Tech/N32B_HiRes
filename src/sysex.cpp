/*
  N32B Hi Res Firmware v3.5.2
  MIT License

  Copyright (c) 2022 SHIK
*/

#include "sysex.h"

void processSysex(unsigned char *data, unsigned int size)
{
    if (size > 3 && data[MANUFACTURER] == SHIK_MANUFACTURER_ID && data[KNOB_INDEX] < NUMBER_OF_KNOBS)
    {
        switch (data[COMMAND])
        {
        case SET_KNOB_AS_CC:
            setKnobAsCC(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX]);
            break;
        case SET_KNOB_AS_DUAL:
            setKnobAsDualCC(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX]);
            break;
        case SET_KNOB_AS_CC_CHANNEL:
            setKnobAsCCWithChannel(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX], data[CHANNEL_INDEX]);
            break;
        case SET_KNOB_AS_NRPN:
            setKnobAsNRPN(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX]);
            break;
        case SET_KNOB_AS_RPN:
            setKnobAsRPN(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX]);
            break;
        case DISABLE_KNOB:
            setKnobAsDisabled(data[KNOB_INDEX]);
            break;
        case HIGH_RES_14BIT:
            useHighResolution(data[KNOB_INDEX], data[KNOB_INDEX]);
            break;
        case SAVE_PRESET:
            savePreset(data[KNOB_INDEX]);
            break;
        case LOAD_PRESET:
            loadPreset(data[KNOB_INDEX]);
            break;
        case SYNC_KNOBS:
            break;
        case CHANGE_CHANNEL:
            handleChangeChannel(data[KNOB_INDEX]);
            break;
        case SEND_CURRENT_CONFIG:
            break;
        default:
            break;
        }
    }
    else
    {
        // Serial.println("Manufacturer ID is not matching the N32B");
    }
}

void setKnobAsCC(byte knobIndex, byte MSB, byte LSB)
{
    activePreset.knobInfo[knobIndex].MSB = MSB;
    activePreset.knobInfo[knobIndex].LSB = LSB;
    activePreset.knobInfo[knobIndex].MODE = KNOB_MODE_STANDARD;
    activePreset.knobInfo[knobIndex].CHANNEL = 128;
}

void setKnobAsDualCC(byte knobIndex, byte MSB, byte LSB)
{
    activePreset.knobInfo[knobIndex].MSB = MSB;
    activePreset.knobInfo[knobIndex].LSB = LSB;
    activePreset.knobInfo[knobIndex].MODE = KNOB_MODE_DUAL;
    activePreset.knobInfo[knobIndex].CHANNEL = 128;
}

void setKnobAsCCWithChannel(byte knobIndex, byte MSB, byte LSB, byte channel)
{
    activePreset.knobInfo[knobIndex].MSB = MSB;
    activePreset.knobInfo[knobIndex].LSB = LSB;
    activePreset.knobInfo[knobIndex].MODE = KNOB_MODE_STANDARD;
    activePreset.knobInfo[knobIndex].CHANNEL = channel;
}

void setKnobAsDisabled(byte knobIndex)
{
    activePreset.knobInfo[knobIndex].MSB = 0;
    activePreset.knobInfo[knobIndex].LSB = 0;
    activePreset.knobInfo[knobIndex].MODE = KNOB_MODE_STANDARD;
    // activePreset.knobInfo[knobIndex].CHANNEL = 17; // Make the knob out of range to disable it
}

void setKnobAsNRPN(byte knobIndex, byte LSB, byte MSB)
{
    activePreset.knobInfo[knobIndex].MSB = MSB;
    activePreset.knobInfo[knobIndex].LSB = LSB;
    activePreset.knobInfo[knobIndex].MODE = KNOB_MODE_NRPN;
    activePreset.knobInfo[knobIndex].CHANNEL = 128;
}

void setKnobAsRPN(byte knobIndex, byte LSB, byte MSB)
{

    activePreset.knobInfo[knobIndex].MSB = MSB;
    activePreset.knobInfo[knobIndex].LSB = LSB;
    activePreset.knobInfo[knobIndex].MODE = KNOB_MODE_RPN;
    activePreset.knobInfo[knobIndex].CHANNEL = 128;
}

void useHighResolution(byte knobIndex, bool value)
{
    activePreset.knobInfo[knobIndex].MODE = KNOB_MODE_HIRES;
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
    if (number < NUMBER_OF_PRESETS)
    {
        loadPreset(number);
    }
}