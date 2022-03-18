// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// ? LCOV_EXCL_START - Exclude from coverage report

#include "LEDAnimation.h"

namespace leka::led::animation {

class Rainbow : public interface::LEDAnimation
{
  public:
	explicit Rainbow() = default;

	void start() final;
	void run() final;
	void stop() final;

	void setLeds(interface::LED &ears, interface::LED &belt) final;

  private:
	interface::LED *_ears		= nullptr;
	interface::LED *_belt		= nullptr;
	uint8_t _step				= 0;
	uint8_t _stage				= 0;
	uint8_t _rainbow_tail_index = 0;

	void stagesRainbow();
	[[nodiscard]] auto getRainbowColor(uint8_t index) const -> RGB;

	void turnLedBlack();
};

}	// namespace leka::led::animation

// ? LCOV_EXCL_STOP - Exclude from coverage report
