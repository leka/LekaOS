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
