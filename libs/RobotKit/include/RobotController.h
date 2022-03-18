// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "platform/mbed_power_mgmt.h"

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceMonitoring.h"

#include "BatteryKit.h"
#include "StateMachine.h"
#include "VideoKit.h"
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
							 interface::FirmwareUpdate &firmware_update, VideoKit &video_kit)
		: _sleep_timeout(sleep_timeout), _battery(battery), _firmware_update(firmware_update), _videokit(video_kit) {};

	void runLaunchingBehavior() final { _videokit.displayImage("/fs/images/logo.jpg"); }

	void startSleepTimeout() final { _sleep_timeout.start(_sleep_timeout_duration); }
	void stopSleepTimeout() final { _sleep_timeout.stop(); }

	void startWaitingBehavior() final
	{
		_videokit.playVideo(
			"/fs/videos/2022_02_14-animation-face-state-waiting-looking-top-right-to-left-without-eyebrows.avi");
	}
	void stopWaitingBehavior() final { _videokit.stopVideo(); }

	void startSleepingBehavior() final
	{
		_videokit.playVideo("/fs/videos/2022_01_17-animation-face-state-yawning-sleeping_without_eyebrows.avi");
	}
	void stopSleepingBehavior() final { _videokit.stopVideo(); }

	auto isCharging() -> bool final
	{
		auto is_charging = _battery.isCharging();

		_service_monitoring.setChargingStatus(is_charging);

		return is_charging;
	};

	void onStartChargingBehavior(uint8_t level)
	{
		_service_battery.setBatteryLevel(level);

		if (level < 25) {
			_videokit.displayImage("/fs/images/battery_0.jpg");
		} else if (level < 50) {
			_videokit.displayImage("/fs/images/battery_red.jpg");
		} else if (level < 75) {
			_videokit.displayImage("/fs/images/battery_yellow_2.jpg");
		} else if (level < 100) {
			_videokit.displayImage("/fs/images/battery_green_3.jpg");
		} else {
			_videokit.displayImage("/fs/images/battery_green_4.jpg");
		}
	}

	void startChargingBehavior() final
	{
		_battery_kit.onDataUpdated([this](uint8_t level) { onStartChargingBehavior(level); });
	}
	void stopChargingBehavior() final
	{
		_battery_kit.onDataUpdated([](uint8_t level) { _service_battery.setBatteryLevel(level); });
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

		system_reset();
	}

	void raise(auto event) { state_machine.process_event(event); };

	void initializeComponents()
	{
		_videokit.initializeScreen();

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

	VideoKit &_videokit;

	BLEKit _ble {};
	inline static BLEServiceDeviceInformation _service_device_information {};
	inline static BLEServiceBattery _service_battery {};
	inline static BLEServiceMonitoring _service_monitoring {};
	inline static auto services =
		std::to_array<interface::BLEService *>({&_service_device_information, &_service_battery, &_service_monitoring});
};

}	// namespace leka
