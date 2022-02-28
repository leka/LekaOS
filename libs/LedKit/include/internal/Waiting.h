// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::animation {

class Waiting : public interface::LEDAnimation
{
  public:
	explicit Waiting(interface::LED &ears, interface::LED &belt);

	void start() final;
	void run() final;
	void stop() final;

	[[nodiscard]] auto mapStep(uint8_t in_max) const -> float;

	void updateRainbow();

	void setColorCurrentMinusOne(uint8_t index, float step_mapped);
	void setColorCurrent(uint8_t index, float step_mapped);
	void setColorCurrentPlusOne(uint8_t index, float step_mapped);
	void setColorCurrentPlusTwo(uint8_t index, float step_mapped);
	void setColorCurrentPlusThree(uint8_t index, float step_mapped);

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step = 0;
	int _stage	  = -2;
};

}	// namespace leka::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
