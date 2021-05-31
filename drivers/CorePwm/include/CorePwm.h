// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_LK_CORE_PWM_OUT_H_
#define _LEKA_OS_DRIVER_LK_CORE_PWM_OUT_H_

#include "drivers/PwmOut.h"

#include "interface/drivers/PwmOut.h"

namespace leka {

class CorePwm : public interface::PwmOut
{
  public:
	explicit CorePwm(mbed::PwmOut &pwm) : _pwm {pwm} {};

	auto read() -> float final;
	void write(float value) final;

  private:
	mbed::PwmOut &_pwm;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_LK_CORE_PWM_OUT_H_
