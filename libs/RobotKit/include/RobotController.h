// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceDeviceInformation.h"

#include "BatteryKit.h"
#include "CoreLED.h"
#include "LedKit.h"
#include "LoadingGreen.h"
#include "LoadingOrange.h"
#include "LoadingRed.h"
#include "LoadingYellow.h"
#include "Sleeping.h"
#include "StateMachine.h"
#include "interface/RobotController.h"
#include "interface/drivers/Battery.h"
#include "interface/drivers/FirmwareUpdate.h"
#include "interface/drivers/Timeout.h"

namespace leka {

template <typename sm_t = boost::sml::sm<system::robot::StateMachine>>
class RobotController : public interface::RobotController
{
  public:
	sm_t state_machine {static_cast<interface::RobotController &>(*this)};

	explicit RobotController(interface::Timeout &sleep_timeout, interface::Battery &battery,
							 interface::FirmwareUpdate &firmware_update, CoreLED<LedKit::kNumberOfLedsEars> &ears,
							 CoreLED<LedKit::kNumberOfLedsBelt> &belt, LedKit &ledkit)
		: _sleep_timeout(sleep_timeout),
		  _battery(battery),
		  _firmware_update(firmware_update),
		  _ears(ears),
		  _belt(belt),
		  _ledkit(ledkit) {};

	void runLaunchingBehavior() final
	{
		// TODO (@yann): Display Leka x APF logo image
	}

	void startSleepTimeout() final { _sleep_timeout.start(_sleep_timeout_duration); }
	void stopSleepTimeout() final { _sleep_timeout.stop(); }

	void startWaitingBehavior() final
	{
		// TODO: Start Waiting animation video
	}
	void stopWaitingBehavior() final
	{
		// TODO: Stop animation video
	}

	void startSleepingBehavior() final
	{
		// TODO (@yann): Start YawningSleeping animation video

		led::animation::Sleeping animation_sleeping(_ears, _belt);
		_ledkit.start(animation_sleeping);
	}
	void stopSleepingBehavior() final
	{
		// TODO (@yann): Stop animation video
		_ledkit.stop();
	}

	auto isCharging() -> bool final { return _battery.isCharging(); };

	void startChargingBehavior() final
	{
		// TODO (@yann): Display battery state image
		_battery_kit.onDataUpdated([this](uint8_t level) {
			_service_battery.setBatteryLevel(level);
			if (level < 25) {
				led::animation::LoadingRed animation_loading_red(_ears, _belt);
				_ledkit.start(animation_loading_red);
			} else if (level < 50) {
				led::animation::LoadingRed animation_loading_red(_ears, _belt);
				_ledkit.start(animation_loading_red);
			} else if (level < 75) {
				led::animation::LoadingOrange animation_loading_orange(_ears, _belt);
				_ledkit.start(animation_loading_orange);
			} else if (level < 100) {
				led::animation::LoadingYellow animation_loading_yellow(_ears, _belt);
				_ledkit.start(animation_loading_yellow);
			} else {
				led::animation::LoadingGreen animation_loading_green(_ears, _belt);
				_ledkit.start(animation_loading_green);
			}
		});
	}
	void stopChargingBehavior() final
	{
		// TODO (@yann): Stop animation video
		_ledkit.stop();
	}

	auto isReadyToUpdate() -> bool final
	{
		return (_battery.isCharging() && _battery.level() > _minimal_battery_level_to_update);
	};

	void applyUpdate() final
	{
		auto firmware_version = FirmwareVersion {.major = 1, .minor = 2, .revision = 3};
		if (_firmware_update.loadUpdate(firmware_version) && _on_update_loaded_callback != nullptr) {
			_on_update_loaded_callback();
		}
	}

	void raise(auto event) { state_machine.process_event(event); };

	void initializeComponents()
	{
		_ble.setServices(services);
		_ble.init();
	}

	void registerOnUpdateLoadedCallback(std::function<void()> const &on_update_loaded_callback)
	{
		_on_update_loaded_callback = on_update_loaded_callback;
	}

	void registerEvents()
	{
		using namespace system::robot::sm;

		// Setup callbacks for monitoring

		_battery_kit.onDataUpdated([](uint8_t level) { _service_battery.setBatteryLevel(level); });

		_battery_kit.startEventHandler();

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
	uint8_t _minimal_battery_level_to_update {25};

	interface::FirmwareUpdate &_firmware_update;
	std::function<void()> _on_update_loaded_callback {};

	BLEKit _ble {};
	inline static BLEServiceDeviceInformation _service_device_information {};
	inline static BLEServiceBattery _service_battery {};
	inline static auto services =
		std::to_array<interface::BLEService *>({&_service_device_information, &_service_battery});

	CoreLED<LedKit::kNumberOfLedsBelt> &_belt;
	CoreLED<LedKit::kNumberOfLedsEars> &_ears;

	LedKit &_ledkit;
};

}	// namespace leka
