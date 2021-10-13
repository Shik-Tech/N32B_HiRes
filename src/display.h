/*
    N32B Firmware v3.0
    MIT License

    Copyright (c) 2021 SHIK
*/

#ifndef N32B_DISPLAY_h
#define N32B_DISPLAY_h

#include <LEDMatrixDriver.hpp>

class N32B_DISPLAY : public LEDMatrixDriver
{
public:
    // DigitLedDisplay display = DigitLedDisplay(16, 10, 15); // Display setup: (DIN, CS, CLK)
    N32B_DISPLAY(int NO_OF_DRIVERS, int LEDMATRIX_CS_PIN) : LEDMatrixDriver(NO_OF_DRIVERS, LEDMATRIX_CS_PIN){};

    // Set the display brightness (1-15)
    void setBright(uint8_t brightness)
    {
        // display.setBright(brightness);
    }

    // Set digits amount
    void setDigitLimit(uint8_t digitLimit)
    {
        // display.setDigitLimit(digitLimit);
    }

    // Auto clear the display
    void updateDisplay(uint8_t readInterval = 255)
    {
        if (millis() - displayOffTimer >= readInterval)
        {
            setDigit(0, 15);
            setDigit(1, 15);
            display();
            clear();
        }
    }

    // Blink the decimal points
    void blinkDot(uint8_t dotSide, uint16_t readInterval = 255)
    {
        // if (millis() - displayOffTimer >= readInterval)
        // {
            setDigit(!dotSide, 15); // 15 = blank
            setDigit(dotSide, 15, true); // 15 = blank
            display();
        // }
        displayOffTimer = millis();
    }

    void showChannelNumber(uint8_t channelNumber)
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

    void showPresetNumber(byte presetNumber)
    {
        setDigit(1, 14); // "P"
        setDigit(0, presetNumber);
        display();
        displayOffTimer = millis();
    }

    void showStartUpAnimation()
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
    void factoryResetAnimation()
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
    void showSaveMessage()
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

private:
    unsigned long displayOffTimer;
};

#endif
