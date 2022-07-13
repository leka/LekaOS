#pragma once

#include <cstdint>

#include "LedKit.h"
#include "RGB.h"
#include "interface/drivers/LED.h"

using namespace std::chrono;

namespace leds {
class LedManager
{
  public:
	LedManager(leka::interface::LED &ears, leka::interface::LED &belt, leka::LedKit &ledkit);

	void turnOnCaptor(uint8_t indexCaptor, leka::RGB color);
	void turnOff();
	void setRandomCaptorWithColor(uint8_t captorNumber);

	void winAnimation();

  private:	 // LedManager():_level(Round{})
	leka::interface::LED &_ears;
	leka::interface::LED &_belt;
	leka::LedKit &_ledKit;

	void turnOnEarLeft(leka::RGB color);
	void turnOnEarRight(leka::RGB color);
	void turnOffEarLeft();
	void turnOffEarRight();
	void turnOnBeltRightBack(leka::RGB color);
	void turnOffBeltRightBack();
	void turnOnBeltRightFront(leka::RGB color);
	void turnOffBeltRightFront();
	void turnOnBeltLeftBack(leka::RGB color);
	void turnOffBeltLeftBack();
	void turnOnBeltLeftFront(leka::RGB color);
	void turnOffBeltLeftFront();
};
}	// namespace leds
