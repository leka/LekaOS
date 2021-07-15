#include "LKVideoKit.h"

using namespace leka;
using namespace std::chrono_literals;

LKVideoKit::LKVideoKit()
	: _coresdram(_hal),
	  // screen + dsi + ltdc
	  _coreltdc(_hal),
	  _coredsi(_hal, _coreltdc),
	  _coreotm(_coredsi, PinName::SCREEN_BACKLIGHT_PWM),
	  _corelcd(_coreotm),
	  // peripherals
	  _coredma2d(_hal),
	  _corejpeg(_hal, std::make_unique<LKCoreJPEGDMAMode>())
{
}

void LKVideoKit::clear()
{
	// TODO
}

void LKVideoKit::display()
{
	// wait for DMA2D to finish transfer
	while (_coredma2d.getHandle().State != HAL_DMA2D_STATE_READY)
		;
	// refresh screen
	_coredsi.refresh();
	// wait for DSI to finish refresh
	while (_coredsi.isBusy())
		;

	if (auto dt = rtos::Kernel::Clock::now() - _last_time; dt < 40ms) {
		rtos::ThisThread::sleep_for(40ms - dt);
	}
	_last_time = rtos::Kernel::Clock::now();
}
