// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

//    Stage 0 +   Stage 1    +  Stage 2   +   Stage 3    +
//            |              |            |              |
//            |              |            |              |
//            |              |            |              |
//            |              |------------|              | --- White
//            |             /|            |\             |
//            |            / |            | \            |
//            |           /  |            |  \           |
//            |          /   |            |   \          |
//            |         /    |            |    \         |
//            |        /     |            |     \        |
//            |       /      |            |      \       |
//            |      /       |            |       \      |
//            |     /        |            |        \     |
//            |    /         |            |         \    |
//            |   /          |            |          \   |
//            |  /           |            |           \  |
//            | /            |            |            \ |
//    --------|/             |            |             \| --- Black
//            |              |            |              |

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class Amazed : public interface::LEDAnimation
{
  public:
	explicit Amazed(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {}

	void start() final;
	void run() final;
	void stop() final;

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step  = 0;
	uint8_t _stage = 0;

	void stage0();
	void stage1();
	void stage2();
	void stage3();

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
