#pragma once

#include <cstdint>

#include "LedKit.h"
#include "Position.h"
#include "RGB.h"
#include "interface/drivers/LED.h"

using namespace std::chrono;

namespace leds {
class LedManager
{
  public:
	LedManager(leka::interface::LED &ears, leka::interface::LED &belt, leka::LedKit &ledkit);

	void turnOnLedGroup(leka::Position indexLedGroup, leka::RGB color);
	void turnOff();
	void setRandomLedGroupWithColor(leka::Position LedGroupNumber);
	void playReinforcer();
	void turnOn(leka::Position component, leka::RGB color);
	void turnOff(leka::Position component);

  private:
	leka::interface::LED &_ears;
	leka::interface::LED &_belt;
	leka::LedKit &_ledKit;
};
}	// namespace leds
