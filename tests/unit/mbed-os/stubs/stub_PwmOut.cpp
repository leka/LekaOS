// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "stub_PwmOut.h"

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

}	// namespace mbed

namespace leka {

float spy_PwmOut_value = -42.0f;

float spy_PwmOut_getValue()
{
	return spy_PwmOut_value;
}

}	// namespace leka
