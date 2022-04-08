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
	explicit Wink() = default;

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

	void stage0();
	void stage1();
	void stage2();

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
