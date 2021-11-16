// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_BATTERY_H_
#define _LEKA_OS_DRIVER_CORE_BATTERY_H_

#include "drivers/AnalogIn.h"
#include "drivers/interfaces/InterfaceDigitalIn.h"

namespace leka {

class CoreBattery
{
  public:
	explicit CoreBattery(PinName pin, mbed::interface::DigitalIn &charge_input)
		: _pin {mbed::AnalogIn(pin, voltage::reference)}, _charge_input(charge_input) {};

	auto getVoltage() -> float;
	auto isInCharge() -> bool;

	struct capacity {
		static constexpr auto full			= float {12.52};
		static constexpr auto three_quarter = float {11.73};
		static constexpr auto half			= float {11.08};
		static constexpr auto quarter		= float {10.47};
		static constexpr auto empty			= float {09.00};
	};

	struct voltage {
		static constexpr auto reference = float {3.33};
	};

	struct polynomial {
		static constexpr auto degree_0 = float {47.5};
		static constexpr auto degree_1 = float {-50.7};
		static constexpr auto degree_2 = float {15.8};
	};

  private:
	auto readRawVoltage() -> float;
	[[nodiscard]] auto convertToRealVoltage(float value) const -> float;

	mbed::AnalogIn _pin;
	mbed::interface::DigitalIn &_charge_input;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_BATTERY_H_
