/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#include "display.h"
// Auto clear the display
void N32B_DISPLAY::updateDisplay(uint8_t readInterval)
{
    if (millis() - displayOffTimer >= readInterval)
    {
        setDigit(0, 15);
        setDigit(1, 15);
        display();
        clear();
    }
}

void N32B_DISPLAY::showValue(int value)
{
    setDigit(1, 15);
    int number = value;
    uint8_t numberOfDigits = 0;
    if (number == 0)
    {
        setDigit(0, 0);
    }
    else
    {
        while (number > 0)
        {
            int digit = number % 10;

            if (value > 99 && numberOfDigits < 1)
            {
                setDigit(numberOfDigits, digit, true);
            }
            else
            {
                setDigit(numberOfDigits, digit);
            }
            number /= 10;
            numberOfDigits++;
        }
    }

    // setIntensity(map(value, 0, 127, 0, 4));
    display();
    displayOffTimer = millis();
}

// Blink the decimal points
void N32B_DISPLAY::blinkDot(uint8_t dotSide)
{
    // setIntensity(0);
    setDigit(!dotSide, 15, true); // 15 = blank
    setDigit(dotSide, 15);        // 15 = blank
    display();
    displayOffTimer = millis();
}

void N32B_DISPLAY::showChannelNumber(uint8_t channelNumber)
{
    if (channelNumber < 10)
    {
        setDigit(0, channelNumber);
        setDigit(1, 15); // 15 = blank
    }
    else
    {
        setDigit(0, channelNumber % 10);
        setDigit(1, 1);
    }

    display();
    displayOffTimer = millis();
}

void N32B_DISPLAY::showPresetNumber(byte presetNumber)
{
    setDigit(1, 14); // "P"
    setDigit(0, presetNumber);
    display();
    displayOffTimer = millis();
}

void N32B_DISPLAY::showStartUpAnimation()
{
    // int x = 0, y = 0; // start top left
    // bool s = true;    // start with led on
    // for (int i = 0; i < 10; i++)
    // {

    //     // toggle current pixel in framebuffer
    //     setPixel(x, y, s);

    //     // move to next pixel
    //     if (x++ >= 16)
    //     {
    //         // Return to left
    //         x = 0;

    //         // start new line
    //         if (y++ >= 8)
    //         {
    //             y = 0;  // need to return to start
    //             s = !s; // toggle led state
    //         }
    //     }

    //     // Flush framebuffer
    //     display();

    //     delay(200);
    // }
    // uint8_t delayTime = 175;
    // uint8_t repeats = 8;
    // for (uint8_t i = 0; i < repeats; i++)
    // {
    //     // display.write(1, B00001000);
    //     // display.write(2, B00001000);

    //     delay(delayTime);
    //     clear();

    //     // display.write(1, B00000001);
    //     // display.write(2, B00000001);
    //     delay(delayTime);
    //     clear();

    //     // display.write(1, B01000000);
    //     // display.write(2, B01000000);
    //     delay(delayTime);
    //     clear();
    // }
}

// Show animation after factory reset (infinity sign animation)
void N32B_DISPLAY::factoryResetAnimation()
{
    uint8_t delayTime = 100;
    uint8_t repeats = 3;

    for (uint8_t i = 0; i < repeats; i++)
    {
        clear();

        // display.write(2, B00010000);
        delay(delayTime);
        clear();

        // display.write(2, B00011000);
        delay(delayTime);
        clear();

        // display.write(2, B00011100);
        delay(delayTime);
        clear();

        // display.write(2, B00001101);
        delay(delayTime);
        clear();

        // display.write(2, B00000101);
        // display.write(1, B00000001);
        delay(delayTime);
        clear();

        // display.write(2, B00000001);
        // display.write(1, B00100001);
        delay(delayTime);
        clear();

        // display.write(1, B01100001);
        delay(delayTime);
        clear();

        // display.write(1, B01100010);
        delay(delayTime);
        clear();

        // display.write(1, B01000010);
        // display.write(2, B00010000);
        delay(delayTime);
        clear();
    }
}

// Show save message (Sv.)
void N32B_DISPLAY::showSaveMessage()
{
    clear();
    // display.write(2, B01011011);
    // display.write(1, B00011100);
    // delay(300);
    // display.write(1, B10011100);
    // delay(300);
    // display.write(1, B00011100);
    // delay(300);
    // display.write(1, B10011100);
    // delay(300);
}
