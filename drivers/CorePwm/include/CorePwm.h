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

	void period(float value) final;

	void suspend() final;
	void resume() final;

	void enableDeepSleep() final { suspend(); }
	void disableDeepSleep() final { resume(); }

  private:
	mbed::PwmOut _pwm;
	bool _is_suspended = false;
};

}	// namespace leka
