// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

//   0  +  1 +  2 +  3 + 4  +  5 + 6  + 7  + 8  + 9  + 10 + 11 + 12 + 13 + 14 + 15 + 16 + 17 + 18 + 19 +
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |    |----|    |    |    |----|    |    |    |----|    |    |    |----|    |    |    |----|    | --- Green1
//  ----|    |    |    |----|    |    |    |----|    |    |    |----|    |    |    |----|    |    |    | --- Green2
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
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |
//      |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----|    |----| --- Black
//      |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |

#pragma once

// LCOV_EXCL_START

#include "LEDAnimation.h"

namespace leka::led::animation {

class BlinkGreen : public interface::LEDAnimation
{
  public:
	explicit BlinkGreen() = default;

	void start() final;
	void run() final;
	void stop() final;

	void setLeds(interface::LED &ears, interface::LED &belt) final;
	auto isRunning() -> bool final;

  private:
	interface::LED *_ears = nullptr;
	interface::LED *_belt = nullptr;
	bool _running		  = false;
	uint8_t _step		  = 0;
	uint8_t _stage		  = 0;

	void stagesBlack();
	void stagesGreen1();
	void stagesGreen2();

	void turnLedBlack();
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
