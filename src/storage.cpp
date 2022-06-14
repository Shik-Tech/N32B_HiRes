/*
  N32B Hi Res Firmware v3.5.2
  MIT License

  Copyright (c) 2022 SHIK
*/

#include "storage.h"

// Check device version
bool isEEPROMvalid()
{
  for (uint8_t i = 3; i > 0; i--)
  {
    uint8_t byte = EEPROM.read(EEPROM.length() - i);
    uint8_t versionDigit = pgm_read_word_near(firmwareVersion + i - 1);
    if (!(byte == versionDigit)) return false;
  }
  return true;
}

void formatFactory()
{
  // Set last used preset to number 0
  EEPROM.update(lastUsedPresetAddress, 0);

  // Create default preset
  Preset_t defaultPreset;
  defaultPreset.channel = 1;

  for (uint8_t i = 0; i < NUMBER_OF_KNOBS; i++)
  {
    uint8_t indexId = pgm_read_word_near(knobsLocation + i);

    defaultPreset.knobInfo[indexId].MSB = i;
    defaultPreset.knobInfo[indexId].LSB = i + 32;
    defaultPreset.knobInfo[indexId].MODE = KNOB_MODE_STANDARD;
    defaultPreset.knobInfo[indexId].CHANNEL = 0;
    defaultPreset.knobInfo[indexId].INVERT_A = false;
    defaultPreset.knobInfo[indexId].INVERT_B = false;
  }

  // Write the default preset to all preset slots
  uint16_t baseAddress = 1;
  for (uint8_t p = 0; p < NUMBER_OF_PRESETS; p++)
  {
    for (uint16_t byteIndex = 0; byteIndex < sizeof(Preset_t); byteIndex++)
    {
      EEPROM.update(baseAddress + p * sizeof(Preset_t) + byteIndex, ((uint8_t *)(&defaultPreset))[byteIndex]);
    }
  }

  // we write the signature so that the device will never rewrite the factory presets
  for (uint8_t i = 3; i > 0; i--)
  {
    uint8_t versionDigit = pgm_read_word_near(firmwareVersion + i - 1);
    EEPROM.update(EEPROM.length() - i, versionDigit);
  }
}

// Loads the specified preset in the RAM and make it the last used preset
void loadPreset(uint8_t presetNumber)
{
  if (presetNumber < NUMBER_OF_PRESETS)
  {
    uint16_t baseAddress = 1 + (presetNumber * sizeof(Preset_t));
    // Read the active preset from EEPROM
    for (uint16_t byteIndex = 0; byteIndex < sizeof(Preset_t); byteIndex++)
    {
      ((uint8_t *)(&activePreset))[byteIndex] = EEPROM.read(baseAddress + byteIndex);
    }

    // Update the last used preset
    currentPresetNumber = presetNumber;
    n32b_display.showPresetNumber(presetNumber, disableKnobs);

    // Save current preset as the active preset.
    // Is commented out to prevent EEPROM from over used
    // EEPROM.update(lastUsedPresetAddress, currentPresetNumber);
  }
}

void savePreset(uint8_t presetNumber)
{
  if (presetNumber < NUMBER_OF_PRESETS)
  {
    uint16_t baseAddress = presetNumber * sizeof(Preset_t) + 1;

    // write the active preset to EEPROM
    for (uint16_t byteIndex = 0; byteIndex < sizeof(Preset_t); byteIndex++)
    {
      EEPROM.update(baseAddress + byteIndex, ((uint8_t *)(&activePreset))[byteIndex]);
    }

    n32b_display.showSaveMessage();
  }
}
