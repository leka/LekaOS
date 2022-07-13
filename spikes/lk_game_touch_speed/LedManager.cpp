#include "LedManager.h"

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "LogKit.h"

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
void LedManager::turnOnEarLeft(RGB color)
{
	_ears.setColorAtIndex(1, color);
	_ears.show();
}
void LedManager::turnOffEarLeft()
{
	_ears.setColorAtIndex(1, RGB::black);
	_ears.show();
}
void LedManager::turnOnEarRight(RGB color)
{
	_ears.setColorAtIndex(0, color);
	_ears.show();
}
void LedManager::turnOffEarRight()
{
	_ears.setColorAtIndex(0, RGB::black);
	_ears.show();
}
void LedManager::turnOnBeltRightBack(RGB color)
{
	_belt.setColorRange(0, 4, color);
	_belt.show();
}
void LedManager::turnOffBeltRightBack()
{
	_belt.setColorRange(0, 4, RGB::black);
	_belt.show();
}
void LedManager::turnOnBeltRightFront(RGB color)
{
	_belt.setColorRange(5, 9, color);
	_belt.show();
}
void LedManager::turnOffBeltRightFront()
{
	_belt.setColorRange(5, 9, RGB::black);
	_belt.show();
}
void LedManager::turnOnBeltLeftBack(RGB color)
{
	_belt.setColorRange(10, 14, color);
	_belt.show();
}
void LedManager::turnOffBeltLeftBack()
{
	_belt.setColorRange(10, 14, RGB::black);
	_belt.show();
}
void LedManager::turnOnBeltLeftFront(RGB color)
{
	_belt.setColorRange(15, 19, color);
	_belt.show();
}
void LedManager::turnOffBeltLeftFront()
{
	_belt.setColorRange(15, 19, RGB::black);
	_belt.show();
}

void LedManager::turnOnCaptor(uint8_t indexCaptor, leka::RGB color)
{
	switch (indexCaptor) {
		case 0:
			turnOnEarLeft(color);
			break;

		case 1:
			turnOnEarRight(color);
			break;

		case 2:
			turnOnBeltRightBack(color);
			break;

		case 3:
			turnOnBeltRightFront(color);
			break;

		case 4:
			turnOnBeltLeftBack(color);
			break;

		case 5:
			turnOnBeltLeftFront(color);
			break;
		default:
			break;
	}
}

void LedManager::setRandomCaptorWithColor(uint8_t captorNumber)
{
	log_info("dans setRandomCaptorWithColor");
	rtos::ThisThread::sleep_for(500ms);
	constexpr RGB targetColor = RGB::pure_blue;

	turnOff();

	turnOnCaptor(captorNumber, targetColor);
}

void LedManager::winAnimation()
{
	log_info("animation::rainbow");
	_ledKit.start(&leka::LedKit::animation::rainbow);
	rtos::ThisThread::sleep_for(4s);
}
}	// namespace leds
