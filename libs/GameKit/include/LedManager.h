#pragma once

#include "LedKit.h"
#include "Position.h"
#include "RGB.h"
#include "interface/drivers/LED.h"
namespace leds {
class LedManager
{
  public:
	LedManager(leka::interface::LED &ears, leka::interface::LED &belt, leka::LedKit &ledkit);

	void playReinforcer();
	void turnOnComponent(leka::Position component, leka::RGB color);
	void turnOffComponent(leka::Position component);
	void turnOffAllComponents();
	void stop();

  private:
	leka::interface::LED &_ears;
	leka::interface::LED &_belt;
	leka::LedKit &_ledKit;
};
}	// namespace leds
