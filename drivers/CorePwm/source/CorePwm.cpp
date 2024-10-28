// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CorePwm.h"

using namespace leka;

auto CorePwm::read() -> float
{
	return _pwm.read();
}

void CorePwm::write(float value)
{
	_pwm.write(value);
}

void CorePwm::period(float value)
{
	_pwm.period(value);
}

void CorePwm::enableDeepSleep()
{
	_pwm.suspend();
}

void CorePwm::disableDeepSleep()
{
	_pwm.resume();
}
