#include "DrawingTools.h"

namespace leka {
namespace draw {
	uint32_t frame_buffer_start_address = LCD_FRAME_BUFFER;
	uint32_t data_address				= frame_buffer_start_address;

	uint32_t default_background_color = 0xffffffff;

	void toolsInit()
	{
		frame_buffer_start_address = leka::hltdc.LayerCfg[1].FBStartAdress;	  // prior init ltdc with display init

		screensaver::set(&screensaver::squareBouncing);
	}

	void clear(uint32_t color)
	{
		data_address = frame_buffer_start_address;

		leka::dma2d::loadDrawing((uint32_t *)data_address, screen_dimension.width, screen_dimension.height, 0, color);
	}

	void filledRectangle(uint16_t x_0, uint16_t y_0, uint16_t width, uint16_t height, uint32_t color)
	{
		// x_0 and y_0 refer to left corner of the rectangle
		uint8_t n_channel = 4;	 // ARGB

		data_address = frame_buffer_start_address + n_channel * (screen_dimension.width * y_0 + x_0);

		leka::dma2d::loadDrawing((uint32_t *)data_address, width, height, screen_dimension.width - width, color);
	}
}	// namespace draw

namespace screensaver {
	using namespace std::chrono;
	void (*pScreenSaver)(seconds duration_sec);

	void run(seconds duration_sec) { pScreenSaver(duration_sec); }

	void set(void (*pscreen_saver)(seconds duration_sec)) { pScreenSaver = pscreen_saver; }

	void squareBouncing(seconds duration_sec)
	{
		mbed::LowPowerTimer timer;

		uint32_t posx  = 0;
		uint32_t posy  = 0;
		uint32_t dirx  = 1;
		uint32_t diry  = 1;
		uint32_t sizex = 100;
		uint32_t sizey = 100;

		uint32_t bg_color = 0xffffff00;

		// square color
		uint8_t alpha = 0xff;
		uint8_t red	  = 0xff;
		uint8_t green = 0x00;
		uint8_t blue  = 0x00;

		draw::clear(bg_color);

		timer.start();
		while (timer.elapsed_time() < duration_sec) {
			// update position
			posx = (posx + dirx);
			posy = (posy + diry);

			// chek for screen limits
			if (posx >= 800 - sizex || posx == 0) {
				dirx *= -1;
			}
			if (posy >= 480 - sizey || posy == 0) {
				diry *= -1;
			}

			// draw the square
			draw::filledRectangle(posx, posy, sizex, sizey, (alpha << 24) | (red << 16) | (green << 8) | (blue));
			// update colors
			if (green == 0) {
				red--;
				blue++;
			}
			if (red == 0) {
				green++;
				blue--;
			}
			if (blue == 0) {
				red++;
				green--;
			}

			rtos::ThisThread::sleep_for(1ms);
		}
	}
}	// namespace screensaver

}	// namespace leka