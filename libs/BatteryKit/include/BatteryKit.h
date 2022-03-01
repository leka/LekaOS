// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/Battery.h"
#include "interface/drivers/Ticker.h"

namespace leka {

class BatteryKit
{
  public:
	explicit BatteryKit(interface::Battery &battery, interface::Ticker &ticker) : _battery(battery), _ticker(ticker) {};

	void start();
	void stop();

	auto level() -> uint8_t;
	auto isCharging() -> bool;
	void onChargeDidStart(mbed::Callback<void()> const &callback);
	void onChargeDidStop(mbed::Callback<void()> const &callback);

	void onDataUpdated(std::function<void(uint8_t)> const &callback);
	void onLowBattery(std::function<void()> const &callback);

  private:
	interface::Battery &_battery;

	interface::Ticker &_ticker;

	std::function<void(uint8_t)> _on_data_updated {};
	std::function<void()> _on_low_battery {};
};

}	// namespace leka
