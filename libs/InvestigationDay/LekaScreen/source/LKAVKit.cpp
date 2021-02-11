#include "LKAVKit.h"

namespace leka {
using namespace mbed;
using namespace std::chrono;

LKAVKit::LKAVKit(LKCoreAV &lk_core_av) : _lk_core_av(lk_core_av)
{
	_lk_core_av.initialize();
}

void LKAVKit::turnOff()
{
	_lk_core_av.turnOff();
}

void LKAVKit::clear(uint32_t color)
{
	Draw::clear(color);
}

void LKAVKit::screensaver()
{
	_screensaver.start();
}

void LKAVKit::displayImage(FIL *file)
{
	_screensaver.stop();
	_lk_core_av.displayImage(file);
}

}	// namespace leka