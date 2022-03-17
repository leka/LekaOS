// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// +                           +                +               +                            +
// |         Stage 0           |    Stage 1     |    Stage 2    |         Stage 3            |
// |                           |                |               |                            |
// |                           |-\              |               |-\                          |  --- White
// |                         -/|  -\            |             -/|  -\                        |
// |                       -/  |    -\          |           -/  |    -\                      |
// |                     -/    |      -\        |         -/    |      -\                    |
// |                   -/      |        -\      |       -/      |        -\                  |
// |                 -/        |          -\    |     -/        |          -\                |
// |               -/          |            -\  |   -/          |            -\              |
// |             -/            |              -\| -/            |              -\            |
// |           -/              |                |/              |                -\          |
// |         -/                |                |               |                  -\        |
// |       -/                  |                |               |                    -\      |
// |     -/                    |                |               |                      -\    |
// |   -/                      |                |               |                        -\  |
// | -/                        |                |               |                          -\|  --- Black
// |/                          |                |               |                            |
// |                           |                |               |                            |

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class Waiting : public interface::LEDAnimation
{
  public:
	explicit Waiting() = default;

	void start() final;
	void run() final;
	void stop() final;

	void setLeds(interface::LED &ears, interface::LED &belt) final;

  private:
	interface::LED *_ears = nullptr;
	interface::LED *_belt = nullptr;
	uint8_t _step		  = 0;
	uint8_t _stage		  = 0;

	void stage0();
	void stage1();
	void stage2();
	void stage3();

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
