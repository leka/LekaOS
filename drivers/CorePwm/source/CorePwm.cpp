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

void CorePwm::suspend()
{
	if (_is_suspended) {
		return;
	}

	_pwm.suspend();
	_is_suspended = true;
}

void CorePwm::resume()
{
	if (!_is_suspended) {
		return;
	}

	_pwm.resume();
	_is_suspended = false;
}
