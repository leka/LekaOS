// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "BLEKit.h"
#include "BLEServiceBattery.h"

#include "BatteryKit.h"
#include "StateMachine.h"
#include "interface/RobotController.h"
#include "interface/drivers/Battery.h"
#include "interface/drivers/Timeout.h"

namespace leka {

template <typename sm_t = boost::sml::sm<system::robot::StateMachine>>
class RobotController : public interface::RobotController
{
  public:
	sm_t state_machine {static_cast<interface::RobotController &>(*this)};

	explicit RobotController(interface::Timeout &sleep_timeout, interface::Battery &battery)
		: _sleep_timeout(sleep_timeout), _battery(battery) {};

	void startSleepTimeout() final { _sleep_timeout.start(_sleep_timeout_duration); }
	void stopSleepTimeout() final { _sleep_timeout.stop(); }

	auto isCharging() -> bool final { return _battery.isCharging(); };

	void raise(auto event) { state_machine.process_event(event); };

	void initializeComponents()
	{
		_ble.setServices(services);
		_ble.init();
	}

	void registerEvents()
	{
		using namespace system::robot::sm;

		// Setup callbacks for monitoring

		_battery_kit.start();

		// Setup callbacks for each State Machine events

		auto on_sleep_timeout = [this]() { raise(event::sleep_timeout_did_end {}); };
		_sleep_timeout.onTimeout(on_sleep_timeout);

		auto on_charge_did_start = [this]() { raise(event::charge_did_start {}); };
		_battery.onChargeDidStart(on_charge_did_start);

		auto on_charge_did_stop = [this]() { raise(event::charge_did_stop {}); };
		_battery.onChargeDidStop(on_charge_did_stop);

		raise(event::setup_complete {});
	};

  private:
	std::chrono::seconds _sleep_timeout_duration {10};
	interface::Timeout &_sleep_timeout;

	interface::Battery &_battery;
	BatteryKit _battery_kit {_battery};

	BLEKit _ble {};
	BLEServiceBattery _service_battery {};
	std::array<interface::BLEService *, 1> services = {&_service_battery};
};

}	// namespace leka
