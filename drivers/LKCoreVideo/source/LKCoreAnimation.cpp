// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreAnimation.h"
#include <array>
#include <map>
#include <random>

#include "rtos/ThisThread.h"

#include "AnimationBouncingSquare.h"
#include "CGAnimation.h"
#include "corevideo_config.h"

using namespace std::chrono;

namespace leka::animation {

LKCoreAnimation::LKCoreAnimation(LKCoreGraphics &coregraphics) : _coregraphics(coregraphics) {}

void LKCoreAnimation::run()
{
	using animation_map_t = std::map<SelectedAnimation, BouncingSquare *>;

	BouncingSquare animation_bouncing_square(_coregraphics);

	animation_map_t animation_map {{SelectedAnimation::bouncing_square, &animation_bouncing_square},
								   {SelectedAnimation::sparkle_stars, &animation_bouncing_square}};

	BouncingSquare *animation = animation_map[SelectedAnimation::bouncing_square];
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

	const uint16_t n_snowflakes	  = 250;
	const uint16_t snowflake_size = 3;

	std::array<LKCoreGraphicsBase::FilledRectangle, n_snowflakes> snowflakes;

	std::random_device generator;
	std::uniform_int_distribution origin_x_distribution(0, lcd::dimension.width - 1);
	std::uniform_int_distribution origin_y_distribution(0, lcd::dimension.height - 1);
	std::uniform_int_distribution snowflake_size_distribution(snowflake_size - 2, snowflake_size + 2);
	std::uniform_int_distribution x_shift_distribution(-1, 2);

	for (int i = 0; i < n_snowflakes; i++) {
		snowflakes[i].origin.x = origin_x_distribution(generator);
		snowflakes[i].origin.y = origin_y_distribution(generator);
		snowflakes[i].width	   = static_cast<uint16_t>(snowflake_size_distribution(generator));
		snowflakes[i].height   = static_cast<uint16_t>(snowflake_size_distribution(generator));
	}

	while (true) {
		for (int i = 0; i < n_snowflakes; i++) {
			_coregraphics.drawRectangle(snowflakes[i], CGColor::black);
			snowflakes[i].origin.x += x_shift_distribution(generator);
			snowflakes[i].origin.y++;
			if (snowflakes[i].origin.y == lcd::dimension.height) {
				snowflakes[i].origin.x = origin_x_distribution(generator);
				snowflakes[i].origin.y = 0;
				snowflakes[i].width	   = static_cast<uint16_t>(snowflake_size_distribution(generator));
				snowflakes[i].height   = static_cast<uint16_t>(snowflake_size_distribution(generator));
			}
			_coregraphics.drawRectangle(snowflakes[i], CGColor::white);
		}

		rtos::ThisThread::sleep_for(30ms);
	}
}

__attribute__((noreturn)) void LKCoreAnimation::runSparkleStars()
{
	_coregraphics.clearScreen(CGColor::black);

	const uint16_t n_stars				= 50;
	const uint16_t star_size			= 3;
	const uint16_t max_brightness_shift = 100;
	uint16_t star_index					= 0;

	std::array<LKCoreGraphicsBase::FilledRectangle, n_stars> stars;
	std::array<Color, n_stars> stars_brightness;

	std::random_device generator;
	std::uniform_int_distribution origin_x_distribution(0, lcd::dimension.width - 1);
	std::uniform_int_distribution origin_y_distribution(0, lcd::dimension.height - 1);
	std::uniform_int_distribution star_size_distribution(star_size - 1, star_size + 1);
	std::uniform_int_distribution brightness_shift_distribution(-max_brightness_shift / 2, max_brightness_shift / 2);

	for (int i = 0; i < n_stars; i++) {
		auto real_star_size = static_cast<uint16_t>(star_size_distribution(generator));
		stars[i].width		= real_star_size;
		stars[i].height		= real_star_size;
	}

	while (true) {
		star_index = (star_index + 1) % n_stars;

		auto new_brightness = stars_brightness[star_index].red + brightness_shift_distribution(generator);

		if (new_brightness < 0x00) {
			stars_brightness[star_index] = CGColor::black;
		} else if (new_brightness > 0xFF) {
			stars_brightness[star_index] = CGColor::white;
		} else {
			stars_brightness[star_index] = {(uint8_t)new_brightness, (uint8_t)new_brightness, (uint8_t)new_brightness};
		}

		_coregraphics.drawRectangle(stars[star_index], stars_brightness[star_index]);

		if (stars_brightness[star_index].red == 0) {
			stars[star_index].origin.x = origin_x_distribution(generator);
			stars[star_index].origin.y = origin_y_distribution(generator);

			auto real_star_size		 = static_cast<uint16_t>(star_size_distribution(generator));
			stars[star_index].width	 = real_star_size;
			stars[star_index].height = real_star_size;
		}

		rtos::ThisThread::sleep_for(10ms);
	}
}

}	// namespace leka::animation
