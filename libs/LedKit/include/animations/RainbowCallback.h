// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "Callback.h"
#include "LEDAnimation.h"

namespace leka::led::animation {

class RainbowCallback : public interface::LEDAnimation
{
  public:
	explicit RainbowCallback() = default;

	void registerCallback(const mbed::Callback<void()> &callback);

	void start() final;
	void run() final;
	void stop() final;

	void setLeds(interface::LED &ears, interface::LED &belt) final;
	auto isRunning() -> bool final;

  private:
	interface::LED *_ears		= nullptr;
	interface::LED *_belt		= nullptr;
	bool _running				= false;
	uint8_t _step				= 0;
	uint8_t _stage				= 0;
	uint8_t _rainbow_tail_index = 0;
	mbed::Callback<void()> _callback;

	void stagesRainbow();
	[[nodiscard]] auto getRainbowColor(uint8_t index) const -> RGB;

	void turnLedBlack();
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
