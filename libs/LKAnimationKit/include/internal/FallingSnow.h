// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_ANIMATION_FALLINGSNOW_H_
#define _LEKA_OS_LIB_ANIMATION_FALLINGSNOW_H_

// MARK: GCOVR_EXCL_START

#include <array>
#include <random>

#include "CGAnimation.h"
#include "LKCoreGraphicsBase.h"
#include "corevideo_config.h"

namespace leka::animation {

namespace falling_snow {
	const uint16_t n_snowflakes			= 250;
	const uint16_t snowflake_size		= 3;
	const uint16_t max_horizontal_shift = 2;
}	// namespace falling_snow

class FallingSnow : public interface::CGAnimation
{
  public:
	explicit FallingSnow(LKCoreGraphicsBase &coregraphics) : _coregraphics(coregraphics) {};

	void start(void) final;
	void run(void) final;
	void stop(void) final;

  private:
	LKCoreGraphicsBase &_coregraphics;

	std::array<LKCoreGraphicsBase::FilledRectangle, falling_snow::n_snowflakes> _snowflakes;

	std::random_device generator;
	std::uniform_int_distribution<int> origin_x_distribution {0, lcd::dimension.width - 1};
	std::uniform_int_distribution<int> origin_y_distribution {0, lcd::dimension.height - 1};
	std::uniform_int_distribution<uint16_t> snowflake_size_distribution {falling_snow::snowflake_size - 2,
																		 falling_snow::snowflake_size + 2};
	std::uniform_int_distribution<int> horizontal_shift_distribution {-falling_snow::max_horizontal_shift + 1,
																	  falling_snow::max_horizontal_shift};

	void updateSnowflakePosition(uint16_t snowflake_index);
	bool snowflakeTouchGround(uint16_t snowflake_index) const;
	void createNewSnowflake(uint16_t snowflake_index);
	void drawSnowflake(LKCoreGraphicsBase::FilledRectangle snowflake);
	void eraseSnowflake(LKCoreGraphicsBase::FilledRectangle snowflake);
};

}	// namespace leka::animation

// MARK: GCOVR_EXCL_STOP

#endif
