// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ANIMATION_SPARKLESTARS_H_
#define _LEKA_OS_LIB_ANIMATION_SPARKLESTARS_H_

#include <array>
#include <random>

#include "CGAnimation.h"
#include "LKCoreGraphicsBase.h"
#include "corevideo_config.h"

namespace leka::animation {

namespace sparkle_stars {
	const uint16_t n_stars				= 40;
	const uint16_t star_size			= 3;
	const uint16_t max_brightness_shift = 40;
}	// namespace sparkle_stars

class SparkleStars : public interface::CGAnimation
{
  public:
	explicit SparkleStars(LKCoreGraphicsBase &coregraphics) : _coregraphics(coregraphics) {};

	void start(void) final;
	void run(void) final;
	void stop(void) final;

  private:
	LKCoreGraphicsBase &_coregraphics;

	std::array<LKCoreGraphicsBase::FilledRectangle, sparkle_stars::n_stars> _stars;
	std::array<CGColor, sparkle_stars::n_stars> _stars_brightness;

	std::random_device generator;
	std::uniform_int_distribution<int> origin_x_distribution {0, lcd::dimension.width - 1};
	std::uniform_int_distribution<int> origin_y_distribution {0, lcd::dimension.height - 1};
	std::uniform_int_distribution<uint16_t> star_size_distribution {sparkle_stars::star_size - 1,
																	sparkle_stars::star_size + 1};
	std::uniform_int_distribution<int> brightness_shift_distribution {-sparkle_stars::max_brightness_shift,
																	  sparkle_stars::max_brightness_shift};

	void updateBrightness(uint16_t star_index);
	bool isBlackStar(uint16_t star_index);
	void createNewStar(uint16_t star_index);
	void drawStar(LKCoreGraphicsBase::FilledRectangle star, CGColor star_brightness);
};

}	// namespace leka::animation

#endif
