#include "LKAVKit.h"

namespace leka {
using namespace mbed;
using namespace std::chrono;

LKAVKit::LKAVKit(LKCoreAV &lk_core_av) : _lk_core_av(lk_core_av)
{
	_lk_core_av.Init();
	leka::draw::toolsInit();
}

void LKAVKit::turnOff()
{
	_lk_core_av.turnOff();
}

void LKAVKit::clear(uint32_t ColorIndex)
{
	leka::draw::clear(ColorIndex);
}

void LKAVKit::screenSaver(std::chrono::seconds duration_sec)
{
	leka::screensaver::run(duration_sec);
}

void LKAVKit::displayImage(FIL *file)
{
	leka::jpeg::display(file);
}

}	// namespace leka