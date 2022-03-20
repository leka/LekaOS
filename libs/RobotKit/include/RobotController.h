// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "platform/mbed_power_mgmt.h"
#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceMonitoring.h"

#include "BatteryKit.h"
#include "BehaviorKit.h"
#include "CoreMotor.h"
#include "LedKit.h"
#include "SerialNumberKit.h"
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
							 SerialNumberKit &serialnumberkit, interface::FirmwareUpdate &firmware_update,
							 CoreMotor &motor_left, CoreMotor &motor_right, LedKit &ledkit, VideoKit &videokit,
							 BehaviorKit &behaviorkit)
		: _sleep_timeout(sleep_timeout),
		  _battery(battery),
		  _serialnumberkit(serialnumberkit),
		  _firmware_update(firmware_update),
		  _motor_left(motor_left),
		  _motor_right(motor_right),
		  _ledkit(ledkit),
		  _videokit(videokit),
		  _behaviorkit(behaviorkit) {};

	void runLaunchingBehavior() final
	{
		using namespace std::chrono_literals;

		_event_queue.call(&_behaviorkit, &BehaviorKit::launching);
		_event_queue.call(&_videokit, &VideoKit::turnOn);
		rtos::ThisThread::sleep_for(3s);
	}

	void startSleepTimeout() final { _sleep_timeout.start(_sleep_timeout_duration); }
	void stopSleepTimeout() final { _sleep_timeout.stop(); }

	void startWaitingBehavior() final
	{
		_event_queue.call(&_behaviorkit, &BehaviorKit::waiting);
		_event_queue.call(&_videokit, &VideoKit::turnOn);
	}
	void stopWaitingBehavior() final { _event_queue.call(&_behaviorkit, &BehaviorKit::stop); }

	void startSleepingBehavior() final
	{
		using namespace std::chrono_literals;

		_event_queue.call(&_behaviorkit, &BehaviorKit::sleeping);
		_event_queue.call(&_videokit, &VideoKit::turnOn);

		_event_queue.call_in(20s, &_videokit, &VideoKit::turnOff);
		_event_queue.call_in(20s, &_ledkit, &LedKit::stop);
	}
	void stopSleepingBehavior() final { _event_queue.call(&_behaviorkit, &BehaviorKit::stop); }

	auto isCharging() -> bool final
	{
		auto is_charging = _battery.isCharging();

		_service_monitoring.setChargingStatus(is_charging);

		return is_charging;
	};

	void onStartChargingBehavior(uint8_t level)
	{
		_service_battery.setBatteryLevel(level);

		if (level < 5) {
			_event_queue.call(&_behaviorkit, &BehaviorKit::chargingZero);
		} else if (level < 25) {
			_event_queue.call(&_behaviorkit, &BehaviorKit::chargingRed);
		} else if (level < 50) {
			_event_queue.call(&_behaviorkit, &BehaviorKit::chargingOrange);
		} else if (level < 75) {
			_event_queue.call(&_behaviorkit, &BehaviorKit::chargingYellow);
		} else {
			_event_queue.call(&_behaviorkit, &BehaviorKit::chargingGreen);
		}
	}

	void startChargingBehavior() final
	{
		using namespace std::chrono_literals;

		_battery_kit.onDataUpdated([this](uint8_t level) { onStartChargingBehavior(level); });
		_event_queue.call(&_videokit, &VideoKit::turnOn);

		_event_queue.call_in(1min, &_videokit, &VideoKit::turnOff);
		_event_queue.call_in(1min, &_ledkit, &LedKit::stop);
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
		_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});

		_ble.setServices(services);
		_ble.init();

		auto _serial_number = _serialnumberkit.getSerialNumber();
		_service_device_information.setSerialNumber(_serial_number);

		_motor_left.stop();
		_motor_right.stop();

		_ledkit.stop();

		_videokit.initializeScreen();
		_videokit.turnOff();
		_videokit.stopVideo();
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

		auto on_low_battery = [this] {
			if (!_battery.isCharging()) {
				_event_queue.call(&_behaviorkit, &BehaviorKit::lowBattery);
			}

			if (_battery.level() == 0) {
				system_reset();
			}
		};
		_battery_kit.onLowBattery(on_low_battery);

		_battery_kit.startEventHandler();

		// Setup callbacks for each State Machine events

		auto on_sleep_timeout = [this]() { raise(event::sleep_timeout_did_end {}); };
		_sleep_timeout.onTimeout(on_sleep_timeout);

		auto on_charge_did_start = [this]() { raise(event::charge_did_start {}); };
		_battery.onChargeDidStart(on_charge_did_start);

		auto on_charge_did_stop = [this]() { raise(event::charge_did_stop {}); };
		_battery.onChargeDidStop(on_charge_did_stop);

		_service_monitoring.onSoftReboot([] { system_reset(); });

		raise(event::setup_complete {});
	};

  private:
	std::chrono::seconds _sleep_timeout_duration {300};
	interface::Timeout &_sleep_timeout;

	interface::Battery &_battery;
	BatteryKit _battery_kit {_battery};
	uint8_t _minimal_battery_level_to_update {25};

	SerialNumberKit &_serialnumberkit;

	interface::FirmwareUpdate &_firmware_update;
	std::function<void()> _on_update_loaded_callback {};

	CoreMotor &_motor_left;
	CoreMotor &_motor_right;
	LedKit &_ledkit;
	VideoKit &_videokit;

	BehaviorKit &_behaviorkit;

	rtos::Thread _thread {};
	events::EventQueue _event_queue {};

	BLEKit _ble {};
	inline static BLEServiceDeviceInformation _service_device_information {};
	inline static BLEServiceBattery _service_battery {};
	inline static BLEServiceMonitoring _service_monitoring {};
	inline static auto services =
		std::to_array<interface::BLEService *>({&_service_device_information, &_service_battery, &_service_monitoring});
};

}	// namespace leka
