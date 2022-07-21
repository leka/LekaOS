// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "drivers/interfaces/InterfaceDigitalIn.h"
#include "drivers/interfaces/InterfaceDigitalOut.h"

#include "interface/drivers/QDAC.h"
#include "interface/drivers/TouchSensor.h"

namespace leka {

class CoreTouchSensor : public interface::TouchSensor
{
  public:
	explicit CoreTouchSensor(mbed::interface::DigitalIn &detect_pin, mbed::interface::DigitalOut &power_mode_pin,
							 interface::QDAC &dac, Channel channel)
		: _detect_pin(detect_pin), _power_mode_pin(power_mode_pin), _sensitivity_pin({dac, channel})
	{
		_detect_pin.mode(PinMode::PullUp);
		_sensitivity_pin.dac.init();
	}
	auto read() -> int final;
	void setPowerMode(PowerMode power_mode) final;
	void setSensitivity(uint16_t value, bool saved = false) final;

  private:
	mbed::interface::DigitalIn &_detect_pin;
	mbed::interface::DigitalOut &_power_mode_pin;
	struct AnalogOut {
		interface::QDAC &dac;
		Channel channel;
	};
	AnalogOut _sensitivity_pin;
};
}	// namespace leka
