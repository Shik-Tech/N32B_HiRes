/*
  N32B Hi Res Firmware v3.6.0
  MIT License

  Copyright (c) 2022 SHIK
*/

#include "sysex.h"

void processSysex(unsigned char *data, unsigned int size)
{
    if (size > 3 && data[MANUFACTURER_INDEX] == SHIK_MANUFACTURER_ID)
    {
        switch (data[COMMAND_INDEX])
        {
        case SET_KNOB_MODE:
            activePreset.knobInfo[data[KNOB_INDEX]].MODE = data[MODE_INDEX];
            if (data[MODE_INDEX] != KNOB_SYSEX)
            {
                activePreset.knobInfo[data[KNOB_INDEX]].MSB = data[MSB_INDEX];
                activePreset.knobInfo[data[KNOB_INDEX]].LSB = data[LSB_INDEX];
                activePreset.knobInfo[data[KNOB_INDEX]].CHANNEL = data[CHANNEL_INDEX];
                activePreset.knobInfo[data[KNOB_INDEX]].INVERT_A = data[INVERT_A_INDEX];
                activePreset.knobInfo[data[KNOB_INDEX]].INVERT_B = data[INVERT_B_INDEX];
            }
            else
            {
                // activePreset.knobInfo[data[KNOB_INDEX]].MSB = midi::encodeSysEx(&data[SYSEX_INDEX], activePreset.knobInfo[data[KNOB_INDEX]].sysExData, sizeof(data[SYSEX_INDEX]));

                /*
                 * TODO: handle Korg special case:
                 * https://github.com/FortySevenEffects/arduino_midi_library/blob/master/doc/sysex-codec.md
                 */
                // void handleSysEx(byte * inData, unsigned inSize)
                // {
                //     // SysEx body data starts at 3rd byte: F0 42 aa bb cc dd F7
                //     // 42 being the hex value of the Korg SysEx ID.
                //     const unsigned dataStartOffset = 2;
                //     const unsigned encodedDataLength = inSize - 3; // Remove F0 42 & F7

                //     // Create a large enough buffer where to decode the message
                //     byte decodedData[64];

                //     const unsigned decodedSize = decodeSysEx(inData + dataStartOffset,
                //                                              decodedData,
                //                                              encodedDataLength,
                //                                              true); // flip header bits
                //     // Do stuff with your message
                // }
            }
            break;
        case SAVE_PRESET:
            savePreset(data[KNOB_INDEX]);
            break;
        case LOAD_PRESET:
            loadPreset(data[KNOB_INDEX]);
            break;
        case CHANGE_CHANNEL:
            handleChangeChannel(data[KNOB_INDEX]);
            break;
        case SEND_FIRMWARE_VERSION:
            sendDeviceFirmwareVersion();
            break;
        case SYNC_KNOBS:
            sendActivePreset();
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

void sendDeviceFirmwareVersion()
{
    uint8_t data[5] = {SHIK_MANUFACTURER_ID, SEND_FIRMWARE_VERSION};

    // Send firmware version
    for (uint8_t i = 3; i > 0; i--)
    {
        data[i + 1] = EEPROM.read(EEPROM.length() - i);
    }
    MIDICoreUSB.sendSysEx(5, data);
}
void sendActivePreset() {
    // Send current preset
    for (uint8_t i = 0; i < NUMBER_OF_KNOBS; i++)
    {
        uint8_t indexId = pgm_read_word_near(knobsLocation + i);
        uint8_t presetData[9] = {
            SHIK_MANUFACTURER_ID,
            SYNC_KNOBS,
            pgm_read_word_near(knobsLocation + i),
            activePreset.knobInfo[indexId].MSB,
            activePreset.knobInfo[indexId].LSB,
            activePreset.knobInfo[indexId].CHANNEL,
            activePreset.knobInfo[indexId].MODE,
            activePreset.knobInfo[indexId].INVERT_A,
            activePreset.knobInfo[indexId].INVERT_B};
        MIDICoreUSB.sendSysEx(9, presetData);
    }
}