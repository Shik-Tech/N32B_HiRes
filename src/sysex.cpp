/*
  N32B Hi Res Firmware v3.6.0
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
        case SET_KNOB_MODE:
            activePreset.knobInfo[data[KNOB_INDEX]].MSB = data[MSB_INDEX];
            activePreset.knobInfo[data[KNOB_INDEX]].LSB = data[LSB_INDEX];
            activePreset.knobInfo[data[KNOB_INDEX]].CHANNEL = data[CHANNEL_INDEX];
            activePreset.knobInfo[data[KNOB_INDEX]].MODE = data[MODE_INDEX];
            activePreset.knobInfo[data[KNOB_INDEX]].INVERT_A = data[INVERT_A_INDEX];
            activePreset.knobInfo[data[KNOB_INDEX]].INVERT_B = data[INVERT_B_INDEX];
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