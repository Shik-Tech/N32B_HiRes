/*
  N32B Hi Res Firmware v3.5.0
  MIT License

  Copyright (c) 2022 SHIK
*/

#ifndef N32B_DISPLAY_h
#define N32B_DISPLAY_h

#include <DigitLedDisplay.h>
#include <Arduino.h>
#include <vector>
#include <bitset>

class N32B_DISPLAY : public DigitLedDisplay
{
public:
  N32B_DISPLAY(uint8_t DIN, uint8_t CS, uint8_t CLK) : DigitLedDisplay(DIN, CS, CLK){};

  void updateDisplay(uint8_t readInterval = 150);
  void showValue(uint8_t);
  void blinkDot(uint8_t);
  void showChannelNumber(uint8_t);
  void showPresetNumber(byte);
  void showStartUpAnimation();
  void factoryResetAnimation();
  void showSaveMessage();
private:
  unsigned long displayOffTimer;
};

#endif
