/*
  N32B Hi Res Firmware v3.0.2
  MIT License

  Copyright (c) 2021 SHIK
*/

#ifndef N32B_DISPLAY_h
#define N32B_DISPLAY_h

// #include <LEDMatrixDriver.hpp>
#include <DigitLedDisplay.h>
#include <Arduino.h>
#include <vector>
#include <bitset>

class N32B_DISPLAY : public DigitLedDisplay
{
public:
  // N32B_DISPLAY(int NO_OF_DRIVERS, int LEDMATRIX_CS_PIN) : LEDMatrixDriver(NO_OF_DRIVERS, LEDMATRIX_CS_PIN){};
  N32B_DISPLAY(uint8_t DIN, uint8_t CS, uint8_t CLK) : DigitLedDisplay(DIN, CS, CLK){};

  // Auto clear the display
  void updateDisplay(uint8_t readInterval = 150);
  void showValue(uint8_t);
  void blinkDot(uint8_t);
  void showChannelNumber(uint8_t);
  void showPresetNumber(byte);
  void showStartUpAnimation();
  // Show animation after factory reset (infinity sign animation)
  void factoryResetAnimation();
  // Show save message (Sv.)
  void showSaveMessage();
  std::vector<std::bitset<4>> decToBcd(uint8_t);
  // int VectorToInt(std::vector<std::bitset<4>>);
private:
  unsigned long displayOffTimer;
};

#endif
