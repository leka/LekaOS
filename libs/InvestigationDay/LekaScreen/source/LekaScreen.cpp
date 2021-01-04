/**
 * @file LekaScreen.cpp
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-26
 *
 * @copyright Copyright (c) 2020
 */

#include "LekaScreen.h"

using namespace mbed;
using namespace std::chrono;

Screen::Screen() : _brightness(SCREEN_BACKLIGHT_PWM)
{
	_brightness.period(0.01f);	 // Set PWM at 1/(0.01 seconds) = 100Hz
	_brightness = 0.50f;
}

void squareBouncing(LekaLCD &lcd)
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

	// initialize and select layer 0
	lcd.LTDC_LayerInit(0);
	lcd.setActiveLayer(0);
	// clear layer 0 in yellow
	lcd.clear(bg_color);

	while (true) {
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
		lcd.fillRect(posx, posy, sizex, sizey, (alpha << 24) | (red << 16) | (green << 8) | (blue));

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

		// HAL_Delay(1);	// ~2ms little delay to let things settle
		rtos::ThisThread::sleep_for(1ms);
	}
}

void Screen::start()
{
	printf("Screen example\n\n");

	while (true) {
		squareBouncing(_lcd);
		rtos::ThisThread::sleep_for(1ms);
	}

	printf("End of Screen example\n\n");
}
