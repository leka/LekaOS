// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// +   Stage 1          +                Stage 2                  +
// |                    |                                         |
// |                    |              -----                      |
// |                    |-----                           -----    |
// |                    |                         -----           |
// |                    |           -----                         |
// |                    |                                    -----|
// |                    |       -----                 -----       |
// |                    |                                         |
// |                    |                      -----              |
// |                    |    -----                                |
// |                    |                  -----                  |
// |                    |                                         |
// |                    |                                         |
// |                    |         Multiple notes of               |
// |                    |          different colors               |
// |                    |        that pop around Leka             |
// |--------------------|                                         | -- Black
// |                    |                                         |
// |                    |                                         |

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class Singing : public interface::LEDAnimation
{
  public:
	explicit Singing(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {};

	void start() final;
	void run() final;
	void stop() final;

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step		 = 0;
	uint8_t _stage		 = 1;
	uint8_t _note_number = 0;

	[[nodiscard]] auto mapStep(uint8_t max_input_value) const -> float;

	void stage1();
	void stage2();

	void increaseBrightnessAtIndex(uint8_t index, RGB color, float position);
	void decreaseBrightnessAtIndex(uint8_t index, RGB color, float position);

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
