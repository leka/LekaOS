// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreEventQueue.h"
#include "interface/drivers/Battery.h"
#include "interface/libs/BatteryKit.h"

namespace leka {

class BatteryKit : public interface::BatteryKit
{
  public:
	explicit BatteryKit(interface::Battery &battery) : _battery(battery) {}

	void startEventHandler() final;

	auto level() -> uint8_t final;
	void onDataUpdated(std::function<void(uint8_t)> const &callback) final;
	void onLowBattery(std::function<void()> const &callback) final;

	auto isCharging() -> bool final;
	void onChargeDidStart(std::function<void()> const &callback) final;
	void onChargeDidStop(std::function<void()> const &callback) final;

  private:
	interface::Battery &_battery;

	CoreEventQueue _event_queue {};

	std::function<void(uint8_t)> _on_data_updated {};
	std::function<void()> _on_low_battery {};
};

}	// namespace leka
