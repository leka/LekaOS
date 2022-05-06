// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "platform/mbed_power_mgmt.h"
#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceCommands.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceFileReception.h"
#include "BLEServiceMonitoring.h"
#include "BLEServiceUpdate.h"

#include "BatteryKit.h"
#include "BehaviorKit.h"
#include "CommandKit.h"
#include "CoreMotor.h"
#include "CoreMutex.h"
#include "FileReception.h"
#include "LedKit.h"
#include "RCLogger.h"
#include "SerialNumberKit.h"
#include "StateMachine.h"
#include "interface/RobotController.h"
#include "interface/drivers/Battery.h"
#include "interface/drivers/FirmwareUpdate.h"
#include "interface/drivers/LCD.hpp"
#include "interface/drivers/Timeout.h"
#include "interface/libs/VideoKit.h"

namespace leka {

template <typename sm_t = boost::sml::sm<system::robot::StateMachine, boost::sml::logger<system::robot::sm::logger>,
										 boost::sml::thread_safe<CoreMutex>>>
class RobotController : public interface::RobotController
{
  public:
	sm_t state_machine {static_cast<interface::RobotController &>(*this), logger};

	explicit RobotController(interface::Timeout &timeout, interface::Battery &battery, SerialNumberKit &serialnumberkit,
							 interface::FirmwareUpdate &firmware_update, CoreMotor &motor_left, CoreMotor &motor_right,
							 LedKit &ledkit, interface::LCD &lcd, interface::VideoKit &videokit,
							 BehaviorKit &behaviorkit, CommandKit &cmdkit)
		: _timeout(timeout),
		  _battery(battery),
		  _serialnumberkit(serialnumberkit),
		  _firmware_update(firmware_update),
		  _motor_left(motor_left),
		  _motor_right(motor_right),
		  _ledkit(ledkit),
		  _lcd(lcd),
		  _videokit(videokit),
		  _behaviorkit(behaviorkit),
		  _cmdkit(cmdkit)
	{
		// nothing to do
	}

	void runLaunchingBehavior() final
	{
		using namespace std::chrono_literals;

		_behaviorkit.launching();
		_lcd.turnOn();
		rtos::ThisThread::sleep_for(3s);
	}

	void startSleepTimeout() final
	{
		using namespace system::robot::sm;
		auto on_sleep_timeout = [this] { raise(event::sleep_timeout_did_end {}); };
		_timeout.onTimeout(on_sleep_timeout);

		_timeout.start(_sleep_timeout_duration);
	}

	void stopSleepTimeout() final { _timeout.stop(); }

	void startWaitingBehavior() final
	{
		_behaviorkit.waiting();
		_lcd.turnOn();
	}

	void stopWaitingBehavior() final { _behaviorkit.stop(); }

	void startSleepingBehavior() final
	{
		using namespace std::chrono_literals;
		using namespace system::robot::sm;

		_behaviorkit.sleeping();
		_lcd.turnOn();

		auto on_sleeping_start_timeout = [this] {
			_event_queue.call(&_lcd, &interface::LCD::turnOff);
			_event_queue.call(&_ledkit, &LedKit::stop);
		};
		_timeout.onTimeout(on_sleeping_start_timeout);

		_timeout.start(20s);
	}

	void stopSleepingBehavior() final
	{
		_timeout.stop();
		_behaviorkit.stop();
	}

	auto isCharging() -> bool final
	{
		auto is_charging = _battery.isCharging();

		_service_monitoring.setChargingStatus(is_charging);

		return is_charging;
	}

	void onChargingBehavior(uint8_t level)
	{
		if (level < 5) {
			_behaviorkit.chargingZero();
		} else if (level < 25) {
			_behaviorkit.chargingRed();
		} else if (level < 50) {
			_behaviorkit.chargingOrange();
		} else if (level < 75) {
			_behaviorkit.chargingYellow();
		} else {
			_behaviorkit.chargingGreen();
		}
	}

	void startChargingBehavior() final
	{
		using namespace std::chrono_literals;
		using namespace system::robot::sm;

		_lcd.turnOn();

		auto on_charging_start_timeout = [this] {
			_event_queue.call(&_lcd, &interface::LCD::turnOff);
			_event_queue.call(&_ledkit, &LedKit::stop);
		};
		_timeout.onTimeout(on_charging_start_timeout);

		_timeout.start(1min);
	}

	void stopChargingBehavior() final { _timeout.stop(); }

	auto isReadyToUpdate() -> bool final
	{
		return (_battery.isCharging() && _battery.level() > _minimal_battery_level_to_update);
	}

	void applyUpdate() final
	{
		auto firmware_version = _service_update.getVersion();
		if (_firmware_update.loadUpdate(firmware_version) && _on_update_loaded_callback != nullptr) {
			_on_update_loaded_callback();
		}

		system_reset();
	}

	void raise(auto event)
	{
		_event_queue.call([this, &event] { state_machine.process_event(event); });
	}

	void initializeComponents()
	{
		_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});

		_ble.setServices(services);
		_ble.init();

		auto _serial_number = _serialnumberkit.getSerialNumber();
		_service_device_information.setSerialNumber(_serial_number);

		_motor_left.stop();
		_motor_right.stop();

		_ledkit.init();

		_videokit.initializeScreen();
		_lcd.turnOff();
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

		_battery_kit.onDataUpdated([this](uint8_t level, bool is_charging) {
			auto advertising_data		 = _ble.getAdvertisingData();
			advertising_data.battery	 = level;
			advertising_data.is_charging = is_charging;
			_ble.setAdvertisingData(advertising_data);

			_service_battery.setBatteryLevel(level);
			if (is_charging) {
				onChargingBehavior(level);
			}
		});

		auto on_low_battery = [this] {
			if (!_battery.isCharging()) {
				_behaviorkit.lowBattery();
			}

			if (_battery.level() == 0) {
				system_reset();
			}
		};
		_battery_kit.onLowBattery(on_low_battery);

		_battery_kit.startEventHandler();

		_ble.onConnectionCallback([this] {
			log_info("BLE connected");
			_behaviorkit.stop();
		});

		_ble.onDisconnectionCallback([this] {
			log_info("BLE disconnected");
			_behaviorkit.stop();
		});

		// Setup callbacks for each State Machine events

		auto on_sleep_timeout = [this]() { raise(event::sleep_timeout_did_end {}); };
		_timeout.onTimeout(on_sleep_timeout);

		auto on_charge_did_start = [this]() { raise(event::charge_did_start {}); };
		_battery.onChargeDidStart(on_charge_did_start);

		auto on_charge_did_stop = [this]() { raise(event::charge_did_stop {}); };
		_battery.onChargeDidStop(on_charge_did_stop);

		_service_monitoring.onSoftReboot([] { system_reset(); });

		auto on_commands_received = [this](std::span<uint8_t> _buffer) {
			raise(event::command_received {});

			if (!isCharging()) {
				stopSleepTimeout();
				startSleepTimeout();

				_cmdkit.push(std::span {_buffer.data(), std::size(_buffer)});
			}
		};
		_service_commands.onCommandsReceived(on_commands_received);

		_service_file_reception.onFilePathReceived(
			[this](std::span<const char> path) { file_reception.setFilePath(path.data()); });
		_service_file_reception.onFileDataReceived(
			[this](std::span<const uint8_t> buffer) { file_reception.onPacketReceived(buffer); });

		auto on_update_requested = [this]() { raise(event::update_requested {}); };
		_service_update.onUpdateRequested(on_update_requested);

		raise(event::setup_complete {});
	}

  private:
	system::robot::sm::logger logger {};

	std::chrono::seconds _sleep_timeout_duration {300};
	interface::Timeout &_timeout;

	interface::Battery &_battery;
	BatteryKit _battery_kit {_battery};
	uint8_t _minimal_battery_level_to_update {25};

	SerialNumberKit &_serialnumberkit;

	interface::FirmwareUpdate &_firmware_update;
	std::function<void()> _on_update_loaded_callback {};

	CoreMotor &_motor_left;
	CoreMotor &_motor_right;
	LedKit &_ledkit;
	interface::LCD &_lcd;
	interface::VideoKit &_videokit;

	BehaviorKit &_behaviorkit;
	CommandKit &_cmdkit;

	rtos::Thread _thread {};
	events::EventQueue _event_queue {};

	FileReception file_reception {};

	BLEKit _ble {};

	BLEServiceBattery _service_battery {};
	BLEServiceCommands _service_commands {};
	BLEServiceDeviceInformation _service_device_information {};
	BLEServiceMonitoring _service_monitoring {};
	BLEServiceFileReception _service_file_reception {};
	BLEServiceUpdate _service_update {};

	std::array<interface::BLEService *, 6> services = {
		&_service_battery,	  &_service_commands,		&_service_device_information,
		&_service_monitoring, &_service_file_reception, &_service_update,
	};
};

}	// namespace leka
