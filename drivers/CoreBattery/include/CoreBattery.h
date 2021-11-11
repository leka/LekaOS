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
		static constexpr auto full			= float {12.52};
		static constexpr auto three_quarter = float {11.73};
		static constexpr auto half			= float {11.08};
		static constexpr auto quarter		= float {10.47};
		static constexpr auto empty			= float {09.00};
	};

	struct voltage {
		static constexpr auto divider	= float {0.129};
		static constexpr auto reference = float {3.33};
	};

  private:
	auto readRawVoltage() -> float;

	mbed::AnalogIn _pin;
};

}	// namespace leka

#endif	 //_LEKA_OS_DRIVER_CORE_BATTERY_H_
