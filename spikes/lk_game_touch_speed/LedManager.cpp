#include "LedManager.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

using namespace leka;
namespace leds {

LedManager::LedManager(interface::LED &ears, interface::LED &belt, LedKit &ledkit)
	: _ears(ears), _belt(belt), _ledKit(ledkit)
{
	_ledKit.init();
}

void LedManager::turnOff()
{
	_ears.setColor(RGB::black);
	_belt.setColor(RGB::black);
	_ears.show();
	_belt.show();
}

void LedManager::turnOn(leka::Position component, leka::RGB color)
{
	switch (component) {
		case leka::Position::ear_left:
			_ears.setColorAtIndex(1, color);
			_ears.show();
			break;
		case leka::Position::ear_right:
			_ears.setColorAtIndex(0, color);
			_ears.show();
			break;
		case leka::Position::belt_back_right:
			_belt.setColorRange(0, 4, color);
			_belt.show();
			break;
		case leka::Position::belt_front_right:
			_belt.setColorRange(5, 9, color);
			_belt.show();
			break;
		case leka::Position::belt_back_left:
			_belt.setColorRange(10, 14, color);
			_belt.show();
			break;
		case leka::Position::belt_front_left:
			_belt.setColorRange(15, 19, color);
			_belt.show();
			break;
		default:
			break;
	}
}

void LedManager::turnOff(leka::Position component)
{
	switch (component) {
		case leka::Position::ear_left:
			_ears.setColorAtIndex(1, RGB::black);
			break;
		case leka::Position::ear_right:
			_ears.setColorAtIndex(0, RGB::black);
			break;
		case leka::Position::belt_back_right:
			_belt.setColorRange(0, 4, RGB::black);
			break;
		case leka::Position::belt_front_right:
			_belt.setColorRange(5, 9, RGB::black);
			break;
		case leka::Position::belt_back_left:
			_belt.setColorRange(10, 14, RGB::black);
			break;
		case leka::Position::belt_front_left:
			_belt.setColorRange(15, 19, RGB::black);
			break;
		default:
			break;
	}
	_ears.show();
	_belt.show();
}

void LedManager::playReinforcer()
{
	_ledKit.start(&leka::LedKit::animation::rainbow);
	rtos::ThisThread::sleep_for(4s);
}
}	// namespace leds
