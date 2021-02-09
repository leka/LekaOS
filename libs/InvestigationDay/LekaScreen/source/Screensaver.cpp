#include "Screensaver.h"

namespace leka {

using namespace rtos;

bool Screensaver::in_run = false;

Screensaver::Screensaver() {}

void Screensaver::start()
{
	in_run			   = true;
	screensaver_thread = new Thread();

	screensaver_thread->start(squareBouncing);
}
void Screensaver::stop()
{
	in_run = false;

	screensaver_thread->join();
}

void Screensaver::squareBouncing()
{
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

	Draw::clear(bg_color);

	while (in_run) {
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
		Draw::filledRectangle(posx, posy, sizex, sizey, (alpha << 24) | (red << 16) | (green << 8) | (blue));

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

		ThisThread::sleep_for(std::chrono::milliseconds(1));
	}
}

}	// namespace leka