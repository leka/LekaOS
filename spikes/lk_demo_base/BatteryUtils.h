// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_BATTERY_UTILS_H_
#define _LEKA_OS_BATTERY_UTILS_H_

#include "drivers/DigitalIn.h"
#include "events/EventQueue.h"

#include "CoreBattery.h"

namespace leka {

class BatteryUtils
{
  public:
	explicit BatteryUtils() = default;

	void registerEventQueue(events::EventQueue &event_queue);

	auto getBatteryLevel() -> float;

  private:
	void checkReboot();
	auto checkRaiseAndDrop() -> bool;

	mbed::DigitalIn charge_input {PinName::BATTERY_CHARGE_STATUS};
	CoreBattery battery {PinName::BATTERY_VOLTAGE, charge_input};

	int loop_since_charge {0};
	int raise_and_drop_counter {0};
	bool last_charge_status {false};
};

}	// namespace leka

#endif	 // _LEKA_OS_BATTERY_UTILS_H_
