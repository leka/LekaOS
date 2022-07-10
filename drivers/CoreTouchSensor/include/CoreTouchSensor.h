// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/interfaces/InterfaceDigitalIn.h"
#include "drivers/interfaces/InterfaceDigitalOut.h"

#include "interface/drivers/QDAC.h"

namespace leka {

class CoreTouchSensor
{
  public:
	explicit CoreTouchSensor(mbed::interface::DigitalIn &detect_pin, mbed::interface::DigitalOut &power_mode_pin,
							 interface::QDAC &dac, uint8_t channel)
		: _detect_pin(detect_pin), _power_mode_pin(power_mode_pin), _sensitivity_pin({dac, channel})
	{
		_detect_pin.mode(PinMode::PullUp);
		_sensitivity_pin.dac.init();
	}
	auto read() -> int;
	void setPowerMode(uint8_t power_mode);
	void adjustSensitivity(uint16_t value, bool saved = false);

  private:
	mbed::interface::DigitalIn &_detect_pin;
	mbed::interface::DigitalOut &_power_mode_pin;
	struct AnalogOut {
		interface::QDAC &dac;
		uint8_t channel;
	};
	AnalogOut _sensitivity_pin;
};
}	// namespace leka
