// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ColorKit.h"
#include "CoreLED.h"
#include "Utils.h"

namespace leka::led::animation::utils {

[[nodiscard]] inline auto normalizeStep(uint8_t step, uint8_t step_max) -> float
{
	return leka::utils::math::map(step, uint8_t {0}, step_max, 0.F, 1.F);
}

}	// namespace leka::led::animation::utils

namespace leka::interface {

class LEDAnimation
{
  public:
	virtual ~LEDAnimation() = default;

	virtual void start() = 0;
	virtual void stop()	 = 0;
	virtual void run()	 = 0;

	virtual void setLeds(interface::LED &ears, interface::LED &belt) = 0;
	virtual auto isRunning() -> bool								 = 0;
};

}	// namespace leka::interface
