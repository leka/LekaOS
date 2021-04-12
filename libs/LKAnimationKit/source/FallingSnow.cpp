// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// MARK: GCOVR_EXCL_START

#include "FallingSnow.h"

namespace leka::animation {

using namespace falling_snow;

void FallingSnow::start(void)
{
	_coregraphics.clearScreen(CGColor::black);

	for (int snowflake_index = 0; snowflake_index < falling_snow::n_snowflakes; snowflake_index++) {
		createNewSnowflake(snowflake_index);
		// _snowflakes[snowflake_index].origin.y = origin_y_distribution(generator);
		_snowflakes[snowflake_index].origin.y = rand() % lcd::dimension.height;
	}
}

void FallingSnow::updateSnowflakePosition(uint16_t snowflake_index)
{
	// _snowflakes[snowflake_index].origin.x += horizontal_shift_distribution(generator);
	_snowflakes[snowflake_index].origin.x += (rand() % 4) - 1;
	_snowflakes[snowflake_index].origin.y++;
}

bool FallingSnow::snowflakeTouchGround(uint16_t snowflake_index) const
{
	return _snowflakes[snowflake_index].origin.y == lcd::dimension.height;
}

void FallingSnow::createNewSnowflake(uint16_t snowflake_index)
{
	// _snowflakes[snowflake_index].origin.x = origin_x_distribution(generator);
	// _snowflakes[snowflake_index].origin.y = 0;
	// _snowflakes[snowflake_index].width	  = snowflake_size_distribution(generator);
	// _snowflakes[snowflake_index].height	  = snowflake_size_distribution(generator);

	_snowflakes[snowflake_index].origin.x = rand() % lcd::dimension.width;
	_snowflakes[snowflake_index].origin.y = 0;
	_snowflakes[snowflake_index].width	  = snowflake_size + (rand() % 5 - 2);
	_snowflakes[snowflake_index].height	  = snowflake_size + (rand() % 5 - 2);
}

void FallingSnow::drawSnowflake(LKCoreGraphicsBase::FilledRectangle snowflake)
{
	_coregraphics.drawRectangle(snowflake, CGColor::white);
}

void FallingSnow::eraseSnowflake(LKCoreGraphicsBase::FilledRectangle snowflake)
{
	_coregraphics.drawRectangle(snowflake, CGColor::black);
}

void FallingSnow::run(void)
{
	for (uint16_t snowflake_index = 0; snowflake_index < n_snowflakes; snowflake_index++) {
		eraseSnowflake(_snowflakes[snowflake_index]);

		updateSnowflakePosition(snowflake_index);

		if (snowflakeTouchGround(snowflake_index)) {
			createNewSnowflake(snowflake_index);
		}

		drawSnowflake(_snowflakes[snowflake_index]);
	}
}

void FallingSnow::stop(void)
{
	_coregraphics.clearScreen(CGColor::black);
}

}	// namespace leka::animation

// MARK: GCOVR_EXCL_STOP
