// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START
#include "LEDAnimation.h"

namespace leka::led::animation {

class FastTwo : public interface::LEDAnimation
{
  public:
	explicit FastTwo() = default;

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

	void animation4();
	void animation5();
	void animation6();
	void animation7();
	void rangeGradient2Colors(const RGB &color1, const RGB &color2);
	void rangeGradient3Colors(const RGB &color1, const RGB &color2, const RGB &color3);
	void rangeGradient4Colors(const RGB &color1, const RGB &color2, const RGB &color3, const RGB &color4);
	void rangeGradient5Colors(const RGB &color1, const RGB &color2, const RGB &color3, const RGB &color4,
							  const RGB &color5);
	void turnLedBlack();
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
