#include "LedManager.h"

using namespace leka;
using namespace std::chrono;

namespace leds {

LedManager::LedManager(interface::LED &ears, interface::LED &belt, LedKit &ledkit)
	: _ears(ears), _belt(belt), _ledKit(ledkit)
{
	_ledKit.init();
}

void LedManager::turnOnComponent(leka::Position component, leka::RGB color)
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
		case leka::Position::belt_right_front:
			_belt.setColorRange(0, 4, color);
			_belt.show();
			break;
		case leka::Position::belt_right_back:
			_belt.setColorRange(5, 9, color);
			_belt.show();
			break;
		case leka::Position::belt_left_back:
			_belt.setColorRange(10, 14, color);
			_belt.show();
			break;
		case leka::Position::belt_left_front:
			_belt.setColorRange(15, 19, color);
			_belt.show();
			break;
		default:
			break;
	}
}

void LedManager::turnOffComponent(leka::Position component)
{
	switch (component) {
		case leka::Position::ear_left:
			_ears.setColorAtIndex(1, RGB::black);
			break;
		case leka::Position::ear_right:
			_ears.setColorAtIndex(0, RGB::black);
			break;
		case leka::Position::belt_right_front:
			_belt.setColorRange(0, 4, RGB::black);
			break;
		case leka::Position::belt_right_back:
			_belt.setColorRange(5, 9, RGB::black);
			break;
		case leka::Position::belt_left_back:
			_belt.setColorRange(10, 14, RGB::black);
			break;
		case leka::Position::belt_left_front:
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
void LedManager::playAnimation(leka::interface::LEDAnimation *animation)
{
	_ledKit.start(animation);
}
void LedManager::stop()
{
	_ledKit.stop();
}
void LedManager::turnOffAllComponents()
{
	_belt.setColor(RGB::black);
	_ears.setColor(RGB::black);
	_belt.show();
	_ears.show();
}

}	// namespace leds
