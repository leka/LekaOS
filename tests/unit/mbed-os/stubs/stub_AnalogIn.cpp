// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "stub_AnalogIn.h"

SingletonPtr<PlatformMutex> mbed::AnalogIn::_mutex;

mbed::AnalogIn::AnalogIn(PinName pin, float vref)
{
	return;
}

float mbed::AnalogIn::read()
{
	return leka::spy_AnalogIn_value;
}

unsigned short mbed::AnalogIn::read_u16()
{
	return static_cast<uint16_t>(leka::spy_AnalogIn_value);
}

void analogin_free(analogin_t *obj)
{
	return;
}

namespace leka {

float spy_AnalogIn_value = -42.0;

float spy_AnalogIn_setValue(float value)
{
	spy_AnalogIn_value = value;
	return spy_AnalogIn_value;
}

}	// namespace leka
