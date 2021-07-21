#ifndef _LEKA_OS_LIB_LKVIDEOKIT_H_
#define _LEKA_OS_LIB_LKVIDEOKIT_H_

#include <chrono>

#include "rtos/ThisThread.h"

#include "CoreDMA2D.h"
#include "CoreDSI.h"
#include "CoreJPEG.h"
#include "CoreLCD.h"
#include "CoreLCDDriverOTM8009A.h"
#include "CoreLTDC.h"
#include "Font.h"
#include "Graphics.h"
#include "LKCoreSTM32Hal.h"

using namespace std::chrono_literals;

namespace leka {

class VideoKit
{
  public:
	VideoKit(LKCoreSTM32HalBase &);

	auto getDSI() -> CoreDSI &;
	auto getLTDC() -> CoreLTDC &;
	auto getDMA2D() -> CoreDMA2D &;
	auto getJPEG() -> CoreJPEG &;

	void initialize();

	void setFrameRateLimit(unsigned framerate);

	void clear(gfx::Color color = gfx::Color::White);

	void draw(gfx::Drawable &drawable);

	void drawRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, gfx::Color color);
	void drawText(const char *text, uint32_t column, uint32_t line, gfx::Color color=gfx::Color::Black, gfx::Color bg_color=gfx::Color::White);

	void display();

  private:
	void refresh();
	void tick();

	LKCoreSTM32HalBase &_hal;

	// peripherals
	CoreJPEG _corejpeg;
	CoreDMA2D _coredma2d;

	// ltdc + dsi + lcd screen
	CoreLTDC _coreltdc;
	CoreDSI _coredsi;
	CoreLCDDriverOTM8009A _coreotm;
	CoreLCD _corelcd;

	rtos::Kernel::Clock::time_point _last_time = rtos::Kernel::Clock::now();
	std::chrono::milliseconds _frametime	   = 40ms;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LKVIDEOKIT_H_
