// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "LEDAnimation.h"

namespace leka::led::animation {

class Sprinkles : public interface::LEDAnimation
{
  public:
	explicit Sprinkles() = default;

	void start() final;
	void run() final;
	void stop() final;

	void setLeds(interface::LED &ears, interface::LED &belt) final;
	auto isRunning() -> bool final;

  private:
	interface::LED *_ears	 = nullptr;
	interface::LED *_belt	 = nullptr;
	bool _running			 = false;
	uint8_t _step			 = 0;
	uint8_t _stage			 = 0;
	uint8_t _wave_tail_index = 0;

	void moveWave(uint8_t &wave_tail_index);
	[[nodiscard]] auto getSprinklesColor() const -> RGB;

	void turnLedBlack();
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
