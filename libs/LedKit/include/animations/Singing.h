// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// +      Stage 0       +                Stage 1                  +
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

// LCOV_EXCL_START

#include "LEDAnimation.h"

namespace leka::led::animation {

class Singing : public interface::LEDAnimation
{
  public:
	explicit Singing() = default;

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
	uint8_t _note_number  = 0;

	void stage0();
	void stage1();

	void increaseBrightnessAtIndex(uint8_t index, RGB color, float position);
	void decreaseBrightnessAtIndex(uint8_t index, RGB color, float position);

	void turnLedBlack();
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
