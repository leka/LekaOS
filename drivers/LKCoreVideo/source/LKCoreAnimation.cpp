// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreAnimation.h"
#include <array>
#include <map>

#include "rtos/ThisThread.h"

#include "AnimationBouncingSquare.h"
#include "CGAnimation.h"
#include "corevideo_config.h"

using namespace std::chrono;

namespace leka {

LKCoreAnimation::LKCoreAnimation(LKCoreGraphics &coregraphics) : _coregraphics(coregraphics) {}

void LKCoreAnimation::run()
{
	using animation_map_t = std::map<SelectedAnimation, AnimationBouncingSquare *>;

	AnimationBouncingSquare animation_bouncing_square(_coregraphics);

	animation_map_t animation_map {{SelectedAnimation::bouncing_square, &animation_bouncing_square},
								   {SelectedAnimation::sparkle_stars, &animation_bouncing_square}};

	AnimationBouncingSquare *animation = animation_map[SelectedAnimation::bouncing_square];
	animation->start();
}

void LKCoreAnimation::run(SelectedAnimation selected_animation)
{
	switch (selected_animation) {
		case bouncing_square:
			break;
		case slow_rainbow:
			runSlowRainbow();
			break;
		case falling_snowflakes:
			runFallingSnowflakes();
			break;
		case sparkle_stars:
			runSparkleStars();
			break;
		default:
			return;
	}
}

void LKCoreAnimation::stop(void) const {}

__attribute__((noreturn)) void LKCoreAnimation::runSlowRainbow()
{
	Color clear_color = {0x00, 0x00, 0x00};

	_coregraphics.clearScreen(CGColor::black);
	rtos::ThisThread::sleep_for(500ms);

	while (true) {
		// Update color
		if (clear_color.red == 0x00 && clear_color.green == 0xFF) {
			if (++clear_color.blue == 0xFF) {
				clear_color.green--;
			}
		} else if (clear_color.red == 0xFF && clear_color.green == 0x00) {
			if (--clear_color.blue == 0x00) {
				clear_color.green++;
			}
		} else if (clear_color.red == 0x00 && clear_color.blue == 0xFF) {
			if (--clear_color.green == 0x00) {
				clear_color.red++;
			}
		} else if (clear_color.red == 0xFF && clear_color.blue == 0x00) {
			if (++clear_color.green == 0xFF) {
				clear_color.red--;
			}
		} else if (clear_color.green == 0x00 && clear_color.blue == 0xFF) {
			if (++clear_color.red == 0xFF) {
				clear_color.blue--;
			}
		} else if (clear_color.green == 0xFF && clear_color.blue == 0x00) {
			if (--clear_color.red == 0x00) {
				clear_color.blue++;
			}
		} else {
			// Get to initial condition
			clear_color.red++;
			clear_color.blue = 0x01;
		}

		// Apply color
		_coregraphics.clearScreen(clear_color);

		// Take a break
		rtos::ThisThread::sleep_for(25ms);
	}
}

__attribute__((noreturn)) void LKCoreAnimation::runFallingSnowflakes()
{
	_coregraphics.clearScreen(CGColor::black);

	const uint8_t n_snowflakes = 250;
	auto snowflake_size		   = 3;

	std::array<LKCoreGraphicsBase::FilledRectangle, n_snowflakes> snowflakes;

	for (int i = 0; i < n_snowflakes; i++) {
		snowflakes[i].origin.x = rand() % lcd::dimension.width;
		snowflakes[i].origin.y = rand() % lcd::dimension.height;
		snowflakes[i].width	   = snowflake_size + (rand() % 5 - 2);
		snowflakes[i].height   = snowflake_size + (rand() % 5 - 2);
	}

	while (true) {
		for (int i = 0; i < n_snowflakes; i++) {
			_coregraphics.drawRectangle(snowflakes[i], CGColor::black);
			snowflakes[i].origin.x += (rand() % 4) - 1;
			snowflakes[i].origin.y++;
			if (snowflakes[i].origin.y == lcd::dimension.height) {
				snowflakes[i].origin.x = rand() % lcd::dimension.width;
				snowflakes[i].origin.y = 0;
				snowflakes[i].width	   = snowflake_size + (rand() % 5 - 2);
				snowflakes[i].height   = snowflake_size + (rand() % 5 - 2);
			}
			_coregraphics.drawRectangle(snowflakes[i], CGColor::white);
		}

		rtos::ThisThread::sleep_for(30ms);
	}
}

__attribute__((noreturn)) void LKCoreAnimation::runSparkleStars()
{
	_coregraphics.clearScreen(CGColor::black);

	const uint8_t n_stars		 = 50;
	uint8_t star_index			 = 0;
	auto star_size				 = 3;
	uint8_t max_brightness_shift = 100;

	std::array<LKCoreGraphicsBase::FilledRectangle, n_stars> stars;
	std::array<Color, n_stars> stars_brightness;

	for (int i = 0; i < n_stars; i++) {
		auto real_star_size = star_size + (rand() % 3 - 1);
		stars[i].width		= real_star_size;
		stars[i].height		= real_star_size;
	}

	while (true) {
		star_index = (star_index + 1) % n_stars;

		auto brightness_shift = (rand() % max_brightness_shift) - (max_brightness_shift / 2);
		auto new_brightness	  = stars_brightness[star_index].red + brightness_shift;

		if (new_brightness < 0x00) {
			stars_brightness[star_index] = CGColor::black;
		} else if (new_brightness > 0xFF) {
			stars_brightness[star_index] = CGColor::white;
		} else {
			stars_brightness[star_index] = {(uint8_t)new_brightness, (uint8_t)new_brightness, (uint8_t)new_brightness};
		}

		_coregraphics.drawRectangle(stars[star_index], stars_brightness[star_index]);

		if (stars_brightness[star_index].red == 0) {
			stars[star_index].origin.x = rand() % lcd::dimension.width;
			stars[star_index].origin.y = rand() % lcd::dimension.height;

			auto real_star_size		 = star_size + (rand() % 3 - 1);
			stars[star_index].width	 = real_star_size;
			stars[star_index].height = real_star_size;
		}

		rtos::ThisThread::sleep_for(10ms);
	}
}

}	// namespace leka
