// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// +   Stage 1      +     Stage 2  +     Stage 3    +
// |                |              |                |
// |                |              |                |
// |                |              |                |
// |                |--------------|                |  --- Pink
// |               /|              |\               |
// |              / |              | \              |
// |             /  |              |  \             |
// |            /   |              |   \            |
// |           /    |              |    \           |
// |          /     |              |     \          |
// |         /      |              |      \         |
// |        /       |              |       \        |
// |       /        |              |        \       |
// |      /         |              |         \      |
// |     /          |              |          \     |
// |    /           |              |           \    |
// |   /            |              |            \   |
// |  /             |              |             \  |
// | /              |              |              \ |
// |/               |              |               \| --- Black
// |                |              |                |

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class Wink : public interface::LEDAnimation
{
  public:
	explicit Wink(interface::LED &ears, interface::LED &belt) : _ears(ears), _belt(belt) {};

	void start() final;
	void run() final;
	void stop() final;

  private:
	interface::LED &_ears;
	interface::LED &_belt;
	uint8_t _step  = 0;
	uint8_t _stage = 1;

	[[nodiscard]] auto mapStep(uint8_t step, uint8_t max_input_value) const -> float;

	void stage1();
	void stage2();
	void stage3();

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report