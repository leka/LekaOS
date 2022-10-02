// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

//  Stage 0 + Stage 1 + 2 +  3  + 4 +  5  + 6 +  7   +   8   +    9    +   ----10       +      11     +
//          |         |   |     |   |     |   |      |       |         |--------------  |             |
//          |         |   |     |   |     |   |      |-------|---------|----------------|             | --- Blue Water
//          |         |   |     |   |     |   |     /|       |----------                |\            |
//          |         |   |     |   |     |   |    / |       |   ----- |                | \           |
//          |         |   |     |   |     |\  |   /  |       |         |                |  \          |
//          |         |   |     |   |    /| \ |  /   |       | Fishes  |    Fishes      |   \         |
//          |         |   |     |\  |   / |  \| /    |       | from    |    from        |    \        |
//          |         |   |    /| \ |  /  |   |/     |       | right   |    left        |     \       |
//          |         |\  |   / |  \| /   |   |      |       |         |                |      \      |
//          |        /| \ |  /  |   |/    |   |      |       |         |                |       \     |
//          |       / |  \| /   |   |     |   |      |       |         |                |        \    |
//          |      /  |   |/    |   |     |   |      |       |         |                |         \   |
//          |     /   |   |     |   |     |   |      |       |         |                |          \  |
//          |    /    |   |     |   |     |   |      |       |         |                |           \ |
//          |   /     |   |     |   |     |   |      |       |         |                |            \|
//          |  /      |   |     |   |     |   |      |       |         |                |             |
//    ------| /       |   |     |   |     |   |      |       |         |                |             | --- Black

#pragma once

// LCOV_EXCL_START

#include "LEDAnimation.h"

namespace leka::led::animation {

class Underwater : public interface::LEDAnimation
{
  public:
	explicit Underwater() = default;

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
	uint8_t step_fish_1	  = 0;
	uint8_t step_fish_2	  = 0;
	uint8_t step_fish_3	  = 0;
	uint8_t step_fish_4	  = 0;
	int position_fish_1	  = -2;
	int position_fish_2	  = -2;
	int position_fish_3	  = -2;
	int position_fish_4	  = -2;

	void stage0();
	void stage1();
	void stage2();
	void stage3();
	void stage4();
	void stage5();
	void stage6();
	void stage7();
	void stage8();
	void stage9();
	void stage10();
	void stage11();

	void showFishAtRight(const RGB &color, int &fish_position, uint8_t fish_speed, uint8_t &step_fish);
	void showFishAtLeft(const RGB &color, int &fish_position, uint8_t fish_speed, uint8_t &step_fish);

	void setColorCurrentMinusOne(int index, float pos, const RGB &color);
	void setColorCurrent(int index, float pos, const RGB &color);
	void setColorCurrentPlusOne(int index, float pos, const RGB &color);
	void setColorCurrentPlusTwo(int index, float pos, const RGB &color);
	void setColorCurrentPlusThree(int index, float pos, const RGB &color);

	void turnLedBlack();
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
