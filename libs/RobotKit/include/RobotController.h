// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "platform/mbed_power_mgmt.h"
#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceCommands.h"
#include "BLEServiceConfig.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceFileExchange.h"
#include "BLEServiceMagicCard.h"
#include "BLEServiceMonitoring.h"
#include "BLEServiceUpdate.h"

#include "ActivityKit.h"
#include "BatteryKit.h"
#include "BehaviorKit.h"
#include "CommandKit.h"
#include "ConfigKit.h"
#include "CoreMutex.h"
#include "FileReception.h"
#include "MagicCard.h"
#include "RCLogger.h"
#include "RFIDKit.h"
#include "SerialNumberKit.h"
#include "StateMachine.h"
#include "interface/RobotController.h"
#include "interface/drivers/Battery.h"
#include "interface/drivers/FirmwareUpdate.h"
#include "interface/drivers/LCD.hpp"
#include "interface/drivers/Motor.h"
#include "interface/drivers/Timeout.h"
#include "interface/libs/LedKit.h"
#include "interface/libs/VideoKit.h"

namespace leka {

using namespace system::robot::sm;

template <typename sm_t = boost::sml::sm<system::robot::StateMachine, boost::sml::logger<system::robot::sm::logger>,
										 boost::sml::thread_safe<CoreMutex>>>
class RobotController : public interface::RobotController
{
  public:
	sm_t state_machine {static_cast<interface::RobotController &>(*this), logger};

	explicit RobotController(interface::Timeout &timeout_state_internal, interface::Timeout &timeout_state_transition,
							 interface::Timeout &timeout_autonomous_activities, interface::Battery &battery,
							 SerialNumberKit &serialnumberkit, interface::FirmwareUpdate &firmware_update,
							 interface::Motor &motor_left, interface::Motor &motor_right, interface::LED &ears,
							 interface::LED &belt, interface::LedKit &ledkit, interface::LCD &lcd,
							 interface::VideoKit &videokit, BehaviorKit &behaviorkit, CommandKit &cmdkit,
							 RFIDKit &rfidkit, ActivityKit &activitykit)
		: _timeout_state_internal(timeout_state_internal),
		  _timeout_state_transition(timeout_state_transition),
		  _timeout_autonomous_activities(timeout_autonomous_activities),
		  _battery(battery),
		  _serialnumberkit(serialnumberkit),
		  _firmware_update(firmware_update),
		  _motor_left(motor_left),
		  _motor_right(motor_right),
		  _ears(ears),
		  _belt(belt),
		  _ledkit(ledkit),
		  _lcd(lcd),
		  _videokit(videokit),
		  _behaviorkit(behaviorkit),
		  _cmdkit(cmdkit),
		  _rfidkit(rfidkit),
		  _activitykit(activitykit)
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
		auto on_sleep_timeout = [this] { raise(event::sleep_timeout_did_end {}); };
		_timeout_state_transition.onTimeout(on_sleep_timeout);

		_timeout_state_transition.start(_sleep_timeout_duration);
	}

	void stopSleepTimeout() final { _timeout_state_transition.stop(); }

	void startDeepSleepTimeout() final
	{
		auto on_deep_sleep_timeout = [this] { raise(event::deep_sleep_timeout_did_end {}); };
		_timeout_state_transition.onTimeout(on_deep_sleep_timeout);

		_timeout_state_transition.start(_deep_sleep_timeout_duration);
	}

	void stopDeepSleepTimeout() final { _timeout_state_transition.stop(); }

	void startIdleTimeout() final
	{
		auto on_idle_timeout = [this] { raise(event::idle_timeout_did_end {}); };
		_timeout_state_transition.onTimeout(on_idle_timeout);

		_timeout_state_transition.start(_idle_timeout_duration);
	}

	void stopIdleTimeout() final { _timeout_state_transition.stop(); }

	void startWaitingBehavior() final
	{
		_behaviorkit.waiting();
		_lcd.turnOn();
	}

	void stopWaitingBehavior() final { _behaviorkit.stop(); }

	void startWorkingBehavior() final
	{
		_behaviorkit.working();
		_lcd.turnOn();
	}

	void startSleepingBehavior() final
	{
		using namespace std::chrono_literals;

		_behaviorkit.sleeping();
		_lcd.turnOn();

		auto on_sleeping_start_timeout = [this] {
			_event_queue.call(&_lcd, &interface::LCD::turnOff);
			_event_queue.call(&_ledkit, &interface::LedKit::stop);
		};
		_timeout_state_internal.onTimeout(on_sleeping_start_timeout);

		_timeout_state_internal.start(20s);
	}

	void stopSleepingBehavior() final
	{
		_timeout_state_internal.stop();
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
		if (level < 25) {
			_behaviorkit.chargingEmpty();
		} else if (level < 50) {
			_behaviorkit.chargingLow();
		} else if (level < 75) {
			_behaviorkit.chargingMedium();
		} else if (level < 90) {
			_behaviorkit.chargingHigh();
		} else if (level == 101) {
			_behaviorkit.chargingFull();
		} else {
			_behaviorkit.chargingHigh();
		}
	}

	void startChargingBehavior() final
	{
		using namespace std::chrono_literals;

		onChargingBehavior(_battery_kit.level());
		_behaviorkit.blinkOnCharge();
		rtos::ThisThread::sleep_for(500ms);
		_lcd.turnOn();

		auto on_charging_start_timeout = [this] { _event_queue.call(&_lcd, &interface::LCD::turnOff); };
		_timeout_state_internal.onTimeout(on_charging_start_timeout);

		_timeout_state_internal.start(1min);
	}

	void stopChargingBehavior() final
	{
		_timeout_state_internal.stop();
		_behaviorkit.stop();
	}

	void startConnectionBehavior() final
	{
		using namespace std::chrono_literals;
		stopActuators();
		if (_battery.isCharging()) {
			_behaviorkit.bleConnectionWithoutVideo();
			rtos::ThisThread::sleep_for(5s);
			_behaviorkit.blinkOnCharge();
		} else {
			_behaviorkit.bleConnectionWithVideo();
			_lcd.turnOn();
		}
	}

	void startDisconnectionBehavior() final
	{
		stopActuators();
		if (_battery.isCharging()) {
			_behaviorkit.blinkOnCharge();
		}
	}

	void startAutonomousActivityMode() final
	{
		auto card = _rfidkit.getLastMagicCardActivated();
		_activitykit.displayMainMenu(card);

		_lcd.turnOn();
	}

	void stopAutonomousActivityMode() final
	{
		_timeout_autonomous_activities.stop();
		_behaviorkit.stop();
		_activitykit.stop();
	}

	void startFileExchange() final
	{
		_behaviorkit.fileExchange();
		if (_battery.isCharging()) {
			_behaviorkit.blinkOnCharge();
		}
		_lcd.turnOn();

		_service_file_exchange.setFileExchangeState(true);

		_service_file_exchange.onFilePathReceived(
			[this](std::span<const char> path) { file_reception.setFilePath(path.data()); });
		_service_file_exchange.onClearFileRequested([this] {
			file_reception.clearFile();
			_service_file_exchange.setFileIsCleared();
		});
		_service_file_exchange.onFileDataReceived(
			[this](std::span<const uint8_t> buffer) { file_reception.onPacketReceived(buffer); });
		_service_file_exchange.onFileSHA256Requested([this](std::span<const char> path) {
			_event_queue.call([this, path] {
				if (FileManagerKit::File file {path.data()}; file.is_open()) {
					_service_file_exchange.setFileSHA256(file.getSHA256());
				}
			});
		});
	}

	void stopFileExchange() final
	{
		_service_file_exchange.setFileExchangeState(false);

		_service_file_exchange.onFilePathReceived(nullptr);
		_service_file_exchange.onClearFileRequested(nullptr);
		_service_file_exchange.onFileDataReceived(nullptr);
		_service_file_exchange.onFileSHA256Requested(nullptr);
	}

	auto isReadyToFileExchange() -> bool final
	{
		auto is_robot_ready = (_battery.isCharging() && _battery.level() > _minimal_battery_level_to_update);

		if (!is_robot_ready) {
			_service_file_exchange.setFileExchangeState(false);
		}

		return is_robot_ready;
	}

	auto isReadyToUpdate() -> bool final
	{
		auto is_robot_ready = _battery.isCharging() && _battery.level() > _minimal_battery_level_to_update;

		auto firmware_version	  = _service_update.getVersion();
		auto is_version_available = _firmware_update.isVersionAvailable(firmware_version);

		return is_robot_ready && is_version_available;
	}

	void applyUpdate() final
	{
		auto firmware_version = _service_update.getVersion();
		if (_firmware_update.loadFirmware(firmware_version) && _on_update_loaded_callback != nullptr) {
			_on_update_loaded_callback();
		}

		system_reset();
	}

	void stopActuatorsAndLcd() final
	{
		_lcd.turnOff();
		stopActuators();
	}

	void suspendHardwareForDeepSleep() final { log_info("TO IMPLEMENT - configuring hardware for deep sleep"); }

	void resetEmergencyStopCounter() final { _emergency_stop_counter = 0; }

	void raise(auto event)
	{
		_event_queue.call([this, &event] { state_machine.process_event(event); });
	}

	void initializeComponents()
	{
		_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});

		_rfidkit.init();

		_ble.setServices(services);
		_ble.init();

		auto _serial_number = _serialnumberkit.getSerialNumber();
		_service_device_information.setSerialNumber(_serial_number);

		auto _os_version = _firmware_update.getCurrentVersion();
		_service_device_information.setOSVersion(_os_version);

		auto short_serial_number_span  = _serialnumberkit.getShortSerialNumber();
		auto short_serial_number_array = std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> {};
		std::copy_n(std::begin(short_serial_number_span), std::size(short_serial_number_span),
					std::begin(short_serial_number_array));
		auto config_robot_name = Config<BLEServiceConfig::kMaxRobotNameSize> {"robot_name", short_serial_number_array};
		_robot_name			   = _configkit.read(config_robot_name);

		_service_config.setRobotName(_robot_name);

		auto advertising_data			  = _ble.getAdvertisingData();
		advertising_data.name			  = reinterpret_cast<const char *>(_robot_name.data());
		advertising_data.version_major	  = _os_version.major;
		advertising_data.version_minor	  = _os_version.minor;
		advertising_data.version_revision = _os_version.revision;

		_ble.setAdvertisingData(advertising_data);

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

	void registerOnFactoryResetNotificationCallback(std::function<void(bool)> const &on_factory_reset_requested)
	{
		_service_update.onFactoryResetNotification(on_factory_reset_requested);
	}

	void raiseEmergencyStop()
	{
		++_emergency_stop_counter;
		raise(event::emergency_stop {});
		if (_emergency_stop_counter >= 7) {
			system_reset();
		}
	}

	void raiseAutonomousActivityModeRequested() { raise(event::autonomous_activities_mode_requested {}); }

	void raiseAutonomousActivityModeExited() { raise(event::autonomous_activities_mode_exited {}); }

	void resetAutonomousActivitiesTimeout()
	{
		_timeout_autonomous_activities.stop();

		auto on_autonomous_activities_timeout = [this] { raise(event::autonomous_activities_mode_exited {}); };
		_timeout_autonomous_activities.onTimeout(on_autonomous_activities_timeout);

		_timeout_autonomous_activities.start(_timeout_autonomous_activities_duration);
	}

	void onMagicCardAvailable(const MagicCard &card)
	{
		using namespace std::chrono;

		// ! TODO: Refactor with composite SM & CoreTimer instead of start/stop
		resetAutonomousActivitiesTimeout();

		auto is_playing				= _activitykit.isPlaying();
		auto NOT_is_playing			= !is_playing;
		auto is_autonomous_mode		= state_machine.is(state::autonomous_activities);
		auto NOT_is_autonomous_mode = !is_autonomous_mode;

		// TODO(@leka/dev-embedded): Refactor startup_delay_elapsed (see #1196)
		const auto startup_delay_elapsed = rtos::Kernel::Clock::now() - kSystemStartupTimestamp > 10s;

		if (card == MagicCard::emergency_stop && startup_delay_elapsed) {
			raiseEmergencyStop();
			return;
		}

		if (card == MagicCard::dice_roll) {
			if (NOT_is_autonomous_mode) {
				start = rtos::Kernel::Clock::now();
				if (start - stop > 1s) {
					raiseAutonomousActivityModeRequested();
				}
			} else if (is_autonomous_mode && NOT_is_playing) {
				stop = rtos::Kernel::Clock::now();
				if (stop - start > 2s) {
					raiseAutonomousActivityModeExited();
				}
			} else {
				start = rtos::Kernel::Clock::now();
				if (start - stop > 1s) {
					raiseAutonomousActivityModeRequested();
				}
				return;
			}
		}

		if (NOT_is_playing && is_autonomous_mode) {
			_activitykit.start(card);
		}
	}

	void registerEvents()
	{
		// Setup callbacks for monitoring

		_rfidkit.onTagActivated([this](const MagicCard &card) {
			// ! IMPORTANT NOTE
			// ! The order of the following functions MUST NOT
			// ! be changed. It is a temporary fix for #1311
			// TODO(@leka/dev-embedded): remove when fixed
			_service_magic_card.setMagicCard(card);
			_behaviorkit.magicCardDetected();
			onMagicCardAvailable(card);
		});

		_activitykit.registerBeforeProcessCallback([this] { resetAutonomousActivitiesTimeout(); });

		_battery_kit.onDataUpdated([this](uint8_t level) {
			auto is_charging = _battery.isCharging();

			auto advertising_data		 = _ble.getAdvertisingData();
			advertising_data.battery	 = level;
			advertising_data.is_charging = static_cast<uint8_t>(is_charging);
			_ble.setAdvertisingData(advertising_data);

			_service_battery.setBatteryLevel(level);
			_service_monitoring.setChargingStatus(is_charging);

			auto is_not_in_file_exchange = !_service_file_exchange.getFileExchangeState();
			if (is_charging && is_not_in_file_exchange) {
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

		_ble.onConnectionCallback([this] { raise(event::ble_connection {}); });

		_ble.onDisconnectionCallback([this] { raise(event::ble_disconnection {}); });

		// Setup callbacks for each State Machine events

		auto on_charge_did_start = [this]() { raise(event::charge_did_start {}); };
		_battery.onChargeDidStart(on_charge_did_start);

		auto on_charge_did_stop = [this]() { raise(event::charge_did_stop {}); };
		_battery.onChargeDidStop(on_charge_did_stop);

		_service_monitoring.onSoftReboot([] { system_reset(); });

		_service_config.onRobotNameUpdated(
			[this](const std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> &robot_name) {
				auto config_robot_name = Config<BLEServiceConfig::kMaxRobotNameSize> {"robot_name"};
				std::ignore			   = _configkit.write(config_robot_name, robot_name);
			});

		auto on_commands_received = [this](std::span<uint8_t> _buffer) {
			raise(event::command_received {});

			if (!isCharging()) {
				stopIdleTimeout();
				startIdleTimeout();

				_cmdkit.push(std::span {_buffer.data(), std::size(_buffer)});
			}
		};
		_service_commands.onCommandsReceived(on_commands_received);

		_service_file_exchange.onSetFileExchangeState([this](bool file_exchange_requested) {
			if (file_exchange_requested) {
				raise(event::file_exchange_start_requested {});
			} else {
				raise(event::file_exchange_stop_requested {});
			}
		});

		auto on_update_requested = [this]() { raise(event::update_requested {}); };
		_service_update.onUpdateRequested(on_update_requested);

		raise(event::setup_complete {});
	}

	void stopActuators()
	{
		_behaviorkit.stop();
		_motor_left.stop();
		_motor_right.stop();
		_ears.hide();
		_belt.hide();
		_videokit.stopVideo();
	}

	auto isBleConnected() -> bool final { return state_machine.is(state::connected); }

  private:
	system::robot::sm::logger logger {};

	interface::Timeout &_timeout_state_internal;

	std::chrono::seconds _sleep_timeout_duration {60};
	std::chrono::seconds _idle_timeout_duration {600};
	std::chrono::seconds _deep_sleep_timeout_duration {600};
	interface::Timeout &_timeout_state_transition;

	interface::Timeout &_timeout_autonomous_activities;
	std::chrono::seconds _timeout_autonomous_activities_duration {600};

	const rtos::Kernel::Clock::time_point kSystemStartupTimestamp = rtos::Kernel::Clock::now();

	rtos::Kernel::Clock::time_point start = rtos::Kernel::Clock::now();
	rtos::Kernel::Clock::time_point stop  = rtos::Kernel::Clock::now();

	interface::Battery &_battery;
	BatteryKit _battery_kit {_battery};
	uint8_t _minimal_battery_level_to_update {25};

	SerialNumberKit &_serialnumberkit;

	ConfigKit _configkit {};
	std::array<uint8_t, BLEServiceConfig::kMaxRobotNameSize> _robot_name {};

	interface::FirmwareUpdate &_firmware_update;
	std::function<void()> _on_update_loaded_callback {};

	interface::Motor &_motor_left;
	interface::Motor &_motor_right;
	interface::LED &_ears;
	interface::LED &_belt;
	interface::LedKit &_ledkit;
	interface::LCD &_lcd;
	interface::VideoKit &_videokit;
	RFIDKit &_rfidkit;
	ActivityKit &_activitykit;

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
	BLEServiceConfig _service_config {};
	BLEServiceMagicCard _service_magic_card {};
	BLEServiceFileExchange _service_file_exchange {};
	BLEServiceUpdate _service_update {};

	std::array<interface::BLEService *, 8> services = {
		&_service_battery, &_service_commands,	 &_service_device_information, &_service_monitoring,
		&_service_config,  &_service_magic_card, &_service_file_exchange,	   &_service_update,
	};

	uint8_t _emergency_stop_counter {0};
};

}	// namespace leka
