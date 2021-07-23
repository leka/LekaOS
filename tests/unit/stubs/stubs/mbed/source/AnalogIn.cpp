// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../AnalogIn.h"

SingletonPtr<PlatformMutex> mbed::AnalogIn::_mutex;

mbed::AnalogIn::AnalogIn(PinName pin, float vref)
{
	// nothing to do
}

void mbed::AnalogIn::set_reference_voltage(float vref)
{
	// nothing to do
}

auto mbed::AnalogIn::read() -> float
{
	return leka::spy_AnalogIn_value;
}

auto mbed::AnalogIn::read_voltage() -> float
{
	return leka::spy_AnalogIn_voltage_value;
}

auto mbed::AnalogIn::read_u16() -> unsigned short
{
	return static_cast<uint16_t>(leka::spy_AnalogIn_value);
}

void analogin_free(analogin_t *obj)
{
	// nothing to do
}

namespace leka {

float spy_AnalogIn_value		 = -42.0;
float spy_AnalogIn_voltage_value = -42.0;

void spy_AnalogIn_setValue(float value)
{
	spy_AnalogIn_value = value;
}

void spy_AnalogIn_setVoltageValue(float value)
{
	spy_AnalogIn_voltage_value = value;
}

}	// namespace leka
