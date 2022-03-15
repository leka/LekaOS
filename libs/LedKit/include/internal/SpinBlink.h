// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

//   0  +  1 +  2 +  3 + 4  +  5 + 6  + 7  + 8  + 9  + 10 + 11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 +
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----| Ears Magenta
//  ----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    | Ears Cyan
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//  ----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    | Belt Magenta
//      |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----| Belt Cyan
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class SpinBlink : public interface::LEDAnimation
{
  public:
	explicit SpinBlink(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {};

	void start() final;
	void run() final;
	void stop() final;

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step  = 0;
	uint8_t _stage = 0;

	[[nodiscard]] auto mapStep(uint8_t step) const -> float;

	void stagesBeltCyan();
	void stagesBeltMagenta();

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
