#ifndef _LEKA_OS_LIB_VIDEOKIT_H_
#define _LEKA_OS_LIB_VIDEOKIT_H_

#include <chrono>
#include <cstdint>

#include "rtos/ThisThread.h"

#include "CGFont.hpp"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreJPEG.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLTDC.hpp"
#include "FileSystemKit.h"
#include "Graphics.h"
#include "LKCoreFatFs.h"
#include "LKCoreSTM32Hal.h"

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

	void setBrightness(float value);

	void setFrameRateLimit(uint32_t framerate);

	void fillJPEGConfig(FileSystemKit::File &file, JPEGConfig *config);

	void clear(gfx::Color color = gfx::Color::White);

	void draw(gfx::Drawable &drawable);

	void drawRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, gfx::Color color);

	void drawText(const char *text, uint32_t x, uint32_t y, gfx::Color color, gfx::Color bg = gfx::Color::Transparent);

	auto drawImage(FileSystemKit::File &file) -> uint32_t;

	auto drawImage(FileSystemKit::File &file, JPEGConfig &config) -> uint32_t;

	void display();

	void resetCounters();
	void displayCounters();

  private:
	void refresh();
	void tick();

	LKCoreSTM32HalBase &_hal;

	// peripherals
	CoreJPEGModeDMA _corejpegmode;
	CoreJPEG _corejpeg;
	CoreDMA2D _coredma2d;

	// ltdc + dsi + lcd screen
	CoreLTDC _coreltdc;
	CoreDSI _coredsi;
	CoreLCDDriverOTM8009A _coreotm;
	CoreLCD _corelcd;

	rtos::Kernel::Clock::time_point _last_time {};
	std::chrono::milliseconds _frametime;

	std::chrono::milliseconds _whole_duration;
	uint32_t _whole_tick;
	std::chrono::milliseconds _maximum;
};

#define VideoKit_DeclareIRQHandlers(instance)                                                                          \
	extern "C" {                                                                                                       \
	void DSI_IRQHandler(void) { HAL_DSI_IRQHandler(&instance.getDSI().getHandle()); }                                  \
	void JPEG_IRQHandler(void) { HAL_JPEG_IRQHandler(&instance.getJPEG().getHandle()); }                               \
	void DMA2_Stream0_IRQHandler(void) { HAL_DMA_IRQHandler(instance.getJPEG().getHandle().hdmain); }                  \
	void DMA2_Stream1_IRQHandler(void) { HAL_DMA_IRQHandler(instance.getJPEG().getHandle().hdmaout); }                 \
	void DMA2D_IRQHandler(void) { HAL_DMA2D_IRQHandler(&instance.getDMA2D().getHandle()); }                            \
	void LTDC_IRQHandler(void) { HAL_LTDC_IRQHandler(&instance.getLTDC().getHandle()); }                               \
	}

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIDEOKIT_H_
