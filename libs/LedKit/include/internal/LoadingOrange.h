// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// +   Stage 1  +  2  +  3  +  4  +  5  +  6  +  7  +      8      +
// |            |     |     |     |     |     |     |             |
// |            |\    |     |\    |     |\    |     |\            |  -- Orange
// |           /| \   |    /| \   |    /| \   |    /| \           |
// |          / |  \  |   / |  \  |   / |  \  |   / |  \          |
// |         /  |   \ |  /  |   \ |  /  |   \ |  /  |   \         |
// |        /   |    \| /   |    \| /   |    \| /   |    \        |
// |       /    |     |/    |     |/    |     |/    |     \       |
// |      /     |     |     |     |     |     |     |      \      |
// |     /      |     |     |     |     |     |     |       \     |
// |    /       |     |     |     |     |     |     |        \    |
// |   /        |     |     |     |     |     |     |         \   |
// |  /         |     |     |     |     |     |     |          \  |
// | /          |     |     |     |     |     |     |           \ |
// |/           |     |     |     |     |     |     |            \|
// |            |     |     |     |     |     |     |             |
// |            |     |     |     |     |     |     |             |  -- Black

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class LoadingOrange : public interface::LEDAnimation
{
  public:
	explicit LoadingOrange(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {};

	void start() final;
	void run() final;
	void stop() final;

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step  = 0;
	uint8_t _stage = 1;

	void stage1();
	void stage2();
	void stage3();
	void stage4();
	void stage5();
	void stage6();
	void stage7();
	void stage8();

	void increaseBrightness();
	void decreaseBrightness(float treshold);

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
