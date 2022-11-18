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
	static constexpr auto default_max_sensitivity_input_value = float {1.F};
	static constexpr auto default_min_sensitivity_input_value = float {0.F};

	explicit CoreTouchSensor(mbed::interface::DigitalIn &detect_pin, mbed::interface::DigitalOut &power_mode_pin,
							 interface::QDAC &dac, uint8_t channel)
		: _detect_pin(detect_pin), _power_mode_pin(power_mode_pin), _sensitivity_pin({dac, channel}) {};
	void init() final;
	auto read() -> bool final;
	void reset() final;
	void setSensitivity(float value) final;

  private:
	enum class PowerMode : uint8_t
	{
		low	   = 0,
		normal = 1
	};
	void setPowerMode(PowerMode power_mode);

	static constexpr auto kDefaultMaxSensitivity = uint16_t {0x0FFF};
	static constexpr auto kDefaultMinSensitivity = uint16_t {0x0000};

	mbed::interface::DigitalIn &_detect_pin;
	mbed::interface::DigitalOut &_power_mode_pin;
	struct AnalogOut {
		interface::QDAC &dac;
		uint8_t channel;
	};
	AnalogOut _sensitivity_pin;

	bool _state {};
};

}	// namespace leka
