// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

// +      Stage 0      + Stage 1 +  Stage 2 + Stage 3+ Stage 4  +     Stage 5       +
// |                   |         |          |        |          |                   |
// |                   |\        |         -|        |         -|                   | --- Red
// |                  /| \       |        / |\       |        / |\                  |
// |                 / |  \      |       /  | \      |       /  | \                 |
// |                /  |   \     |      /   |  \     |      /   |  \                |
// |               /   |    \    |     /    |   \    |     /    |   \               |
// |              /    |     \   |    /     |    \   |    /     |    \              |
// |             /     |      \  |   /      |     \  |   /      |     \             |
// |            /      |       \ |  /       |      \ |  /       |      \            |
// |           /       |        \| /        |       \| /        |       \           |
// |          /        |         |/         |        |/         |        \          |
// |         /         |         |          |        |          |         \         |
// |        /          |         |          |        |          |          \        |
// |       /           |         |          |        |          |           \       |
// |      /            |         |          |        |          |            \      |
// |     /             |         |          |        |          |             \     |
// |    /              |         |          |        |          |              \    |
// |   /               |         |          |        |          |               \   |
// |  /                |         |          |        |          |                \  |
// | /                 |         |          |        |          |                 \ |
// |/                  |         |          |        |          |                  \| --- Black
// |                   |         |          |        |          |                   |
// |                   |         |          |        |          |                   |

#pragma once

// LCOV_EXCL_START

#include "LEDAnimation.h"

namespace leka::led::animation {

class AngryShort : public interface::LEDAnimation
{
  public:
	explicit AngryShort() = default;

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
	void stage3();
	void stage4();
	void stage5();

	void increaseBrightness(float treshold);
	void decreaseBrightness(float treshold);

	void turnLedBlack();
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
