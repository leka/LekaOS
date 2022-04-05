// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/PwmOut.h"

#include "interface/drivers/PwmOut.h"

namespace leka {

class CorePwm : public interface::PwmOut
{
  public:
	explicit CorePwm(PinName pin) : _pwm(mbed::PwmOut(pin)) {}

	auto read() -> float final;
	void write(float value) final;

  private:
	mbed::PwmOut _pwm;
};

}	// namespace leka
