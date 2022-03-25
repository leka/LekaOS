// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BatteryKit.h"

using namespace leka;
using namespace std::chrono_literals;

void BatteryKit::startEventHandler()
{
	_event_queue.dispatch_forever();

	auto on_tick = [this] {
		if (_on_low_battery && level() <= 5) {
			_on_low_battery();
		}

		if (_on_data_updated) {
			_on_data_updated(level());
		}
	};

	_event_queue.call_every(1s, on_tick);
}

auto BatteryKit::level() -> uint8_t
{
	return _battery.level();
}

auto BatteryKit::isCharging() -> bool
{
	return _battery.isCharging();
}

void BatteryKit::onChargeDidStart(mbed::Callback<void()> const &callback)
{
	_battery.onChargeDidStart(callback);
}

void BatteryKit::onChargeDidStop(mbed::Callback<void()> const &callback)
{
	_battery.onChargeDidStop(callback);
}

void BatteryKit::onDataUpdated(std::function<void(uint8_t)> const &callback)
{
	_on_data_updated = callback;
}

void BatteryKit::onLowBattery(std::function<void()> const &callback)
{
	_on_low_battery = callback;
}
