// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SparkleStars.h"

namespace leka::animation {

using namespace sparkle_stars;

void SparkleStars::start(void)
{
	_coregraphics.clearScreen(CGColor::black);
}

void SparkleStars::updateBrightness(uint8_t star_index)
{
	// auto new_brightness	  = _stars_brightness[star_index].red + brightness_shift_distribution(generator);
	auto brightness_shift = (rand() % max_brightness_shift) - (max_brightness_shift / 2);
	auto new_brightness	  = _stars_brightness[star_index].red + brightness_shift;

	if (new_brightness < 0x00) {
		_stars_brightness[star_index] = CGColor::black;
	} else if (new_brightness > 0xFF) {
		_stars_brightness[star_index] = CGColor::white;
	} else {
		_stars_brightness[star_index] = {(uint8_t)new_brightness, (uint8_t)new_brightness, (uint8_t)new_brightness};
	}
}

bool SparkleStars::isBlackStar(uint8_t star_index)
{
	return _stars_brightness[star_index].red == 0;
}

void SparkleStars::createNewStar(uint8_t star_index)
{
	// _stars[star_index].origin.x = origin_x_distribution(generator);
	_stars[star_index].origin.x = rand() % lcd::dimension.width;
	// _stars[star_index].origin.y = origin_y_distribution(generator);
	_stars[star_index].origin.y = rand() % lcd::dimension.height;

	// auto real_star_size		  = star_size_distribution(generator);
	auto real_star_size		  = star_size + (rand() % 3 - 1);
	_stars[star_index].width  = real_star_size;
	_stars[star_index].height = real_star_size;

	_stars_brightness[star_index] = {0x00, 0x00, 0x00};
}

void SparkleStars::drawStar(LKCoreGraphicsBase::FilledRectangle star, Color star_brightness)
{
	_coregraphics.drawRectangle(star, star_brightness);
}

void SparkleStars::run(void)
{
	for (uint8_t star_index = 0; star_index < n_stars; star_index++) {
		updateBrightness(star_index);

		drawStar(_stars[star_index], _stars_brightness[star_index]);

		if (isBlackStar(star_index)) {
			createNewStar(star_index);
		}
	}
}

void SparkleStars::stop(void)
{
	_coregraphics.clearScreen(CGColor::black);
}

}	// namespace leka::animation
