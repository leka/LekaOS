// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START
#include "LEDAnimation.h"

namespace leka::led::animation {

class FastThree : public interface::LEDAnimation
{
  public:
	explicit FastThree() = default;

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

	void turnLedBlack();
	void animation_v2_3_38();
	void animation_v2_4_07();
	void animation_v2_8_17();
	void animation_v2_9_58();
	void animation_v2_11_45();

	[[nodiscard]] auto getHColor(uint8_t index) const -> RGB;
	[[nodiscard]] auto getTransparencyColor(RGB color, uint8_t index_transparency) -> RGB;
	[[nodiscard]] auto getBrightnessColor(RGB color, uint8_t index_brightness) -> RGB;
	[[nodiscard]] auto getModulateRGB(RGB color, uint8_t index_transparency, uint8_t index_brightness) -> RGB;
	[[nodiscard]] auto getHsvValue(uint8_t index_rainbow, uint8_t index_transparency, uint8_t index_brightness) -> RGB;
	[[nodiscard]] auto random8(uint8_t min, uint8_t max) -> uint8_t;
};

}	// namespace leka::led::animation

// LCOV_EXCL_STOP
