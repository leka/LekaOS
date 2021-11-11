// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORE_BATTERY_H_
#define _LEKA_OS_DRIVER_CORE_BATTERY_H_

#include "drivers/AnalogIn.h"

namespace leka {

class CoreBattery
{
  public:
	explicit CoreBattery(PinName pin) : _pin {mbed::AnalogIn(pin, voltage::reference)} {};

	auto getVoltage() -> float;

	struct capacity {
		static constexpr auto max = float {12.60};
		static constexpr auto min = float {7.50};
	};

	struct resistor {
		// TODO (@Benjamin) - find the resistor values, call Mikael
		static constexpr auto r1 = float {47};
		static constexpr auto r2 = float {169};
	};

	struct voltage {
		// TODO (@Benjamin) - should be float {resistor::r1 / (resistor::r1 + resistor::r2)};
		static constexpr auto divider	= float {0.129};
		static constexpr auto reference = float {3.33};
	};

  private:
	auto readRawVoltage() -> float;

	mbed::AnalogIn _pin;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_BATTERY_H_
