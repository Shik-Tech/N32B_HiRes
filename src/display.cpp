/*
  N32B Hi Res Firmware v3.6.0
  MIT License

  Copyright (c) 2022 SHIK
*/

#include "display.h"

const static byte chars[] = {
    B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000, B01111111, B01111011};

// Auto clear the display
void N32B_DISPLAY::clearDisplay(uint8_t readInterval)
{
    if (millis() - displayOffTimer >= readInterval)
    {
        clear();
    }
}

void N32B_DISPLAY::showValue(uint8_t value)
{
    clear();
    printDigit(value);
    if (value > 99)
    {
        write(1, chars[value % 10] | B10000000);
    }
    displayOffTimer = millis();
}

// Blink the decimal points
void N32B_DISPLAY::blinkDot(uint8_t dotSide)
{
    clear();
    write(dotSide, B10000000);
    displayOffTimer = millis();
}

void N32B_DISPLAY::showChannelNumber(uint8_t channelNumber)
{
    clear();
    printDigit(channelNumber);
    displayOffTimer = millis();
}

void N32B_DISPLAY::showPresetNumber(uint8_t presetNumber)
{
    clear();
    printDigit(presetNumber + 1);
    write(2, B01100111);
    displayOffTimer = millis();
}

void N32B_DISPLAY::showStartUpAnimation()
{
    uint8_t delayTime = 160;
    uint8_t repeats = 5;
    for (uint8_t i = 0; i < repeats; i++)
    {
        write(1, B00000001);
        write(2, B00001000);

        delay(delayTime);
        clear();

        write(1, B01000000);
        write(2, B00000001);
        delay(delayTime);
        clear();

        write(1, B00001000);
        write(2, B01000000);
        delay(delayTime);
        clear();
    }
    displayOffTimer = millis();
}

// Show animation after factory reset (infinity symbol animation)
void N32B_DISPLAY::factoryResetAnimation()
{
    uint8_t delayTime = 100;
    uint8_t repeats = 3;

    for (uint8_t i = 0; i < repeats; i++)
    {
        clear();

        write(2, B00010000);
        delay(delayTime);
        clear();

        write(2, B00011000);
        delay(delayTime);
        clear();

        write(2, B00011100);
        delay(delayTime);
        clear();

        write(2, B00001101);
        delay(delayTime);
        clear();

        write(2, B00000101);
        write(1, B00000001);
        delay(delayTime);
        clear();

        write(2, B00000001);
        write(1, B00100001);
        delay(delayTime);
        clear();

        write(1, B01100001);
        delay(delayTime);
        clear();

        write(1, B01100010);
        delay(delayTime);
        clear();

        write(1, B01000010);
        write(2, B00010000);
        delay(delayTime);
        clear();
    }
}

// Show save message "Sv."
void N32B_DISPLAY::showSaveMessage()
{
    clear();
    write(2, B01011011);
    write(1, B00011100);
    delay(300);
    write(1, B10011100);
    delay(300);
    write(1, B00011100);
    delay(300);
    write(1, B10011100);
    delay(300);
}
