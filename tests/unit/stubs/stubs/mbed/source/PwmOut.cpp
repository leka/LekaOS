// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../PwmOut.h"

namespace mbed {

PwmOut::PwmOut(PinName)
{
	return;
}

PwmOut::~PwmOut()
{
	return;
}

void PwmOut::write(float value)
{
	leka::spy_PwmOut_value = value;
}

auto PwmOut::read() -> float
{
	return leka::spy_PwmOut_value;
}

void PwmOut::period(float value)
{
	leka::spy_PwmOut_period = value;
}

}	// namespace mbed

namespace leka {

float spy_PwmOut_value = -42.0f;

float spy_PwmOut_getValue()
{
	return spy_PwmOut_value;
}

float spy_PwmOut_period = -42.0f;

float spy_PwmOut_getPeriod()
{
	return spy_PwmOut_period;
}

}	// namespace leka
