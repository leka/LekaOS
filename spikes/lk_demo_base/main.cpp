// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "events/EventQueue.h"
#include "rtos/EventFlags.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "BLEUtils.h"

#include "Assets.h"
#include "BatteryUtils.h"
#include "DisplayUtils.h"
#include "Flags.h"
#include "HelloWorld.h"
#include "LedsUtils.h"
#include "LogKit.h"
#include "MotorsUtils.h"
#include "RFIDUtils.h"
#include "WatchdogUtils.h"
#include "WifiUtils.h"

using namespace leka;
using namespace std::chrono_literals;

auto thread_watchdog	= rtos::Thread {osPriorityNormal};
auto thread_event_queue = rtos::Thread {osPriorityNormal};
auto thread_ble			= rtos::Thread {osPriorityNormal};
auto thread_video		= rtos::Thread {osPriorityNormal};
auto thread_leds		= rtos::Thread {osPriorityNormal};
auto thread_motors		= rtos::Thread {osPriorityNormal};
auto thread_rfid		= rtos::Thread {osPriorityNormal};
auto thread_wifi		= rtos::Thread {osPriorityNormal};

auto event_queue					  = events::EventQueue {};
auto event_flags_external_interaction = rtos::EventFlags {};

auto hello = HelloWorld {};

auto battery_utils = BatteryUtils {};

auto leds_utils			 = LedsUtils {};
auto selected_reinforcer = LedsReinforcer::glitters;

auto motor_left_dir_1  = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
auto motor_left_dir_2  = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
auto motor_left_speed  = CorePwm {MOTOR_LEFT_PWM};
auto motor_right_dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
auto motor_right_dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
auto motor_right_speed = CorePwm {MOTOR_RIGHT_PWM};
auto motor_left		   = CoreMotor {motor_left_dir_1, motor_left_dir_2, motor_left_speed};
auto motor_right	   = CoreMotor {motor_right_dir_1, motor_right_dir_2, motor_right_speed};
auto motors_utils	   = MotorsUtils {motor_left, motor_right, motor_left_speed, motor_right_speed};

auto ble_utils = BLEUtils {event_flags_external_interaction};

auto hal	   = LKCoreSTM32Hal {};
auto coresdram = CoreSDRAM {hal};
auto display   = VideoKit {hal};
VideoKit_DeclareIRQHandlers(display);
auto display_utils = DisplayUtils {thread_video, event_flags_external_interaction, hal, coresdram, display};

std::string wifi_ssid = "RUT950_LEKA";
std::string wifi_pass = "Leka2021";

void ledsLoop()
{
	while (true) {
		event_flags_external_interaction.wait_any(ENABLE_LEDS_FLAG);
		event_flags_external_interaction.set(ENABLE_LEDS_FLAG);

		leds_utils.runReinforcer(selected_reinforcer);

		// rtos::ThisThread::sleep_for(300ms);
	}
}

void turnOffAllActuators()
{
	motors_utils.stop();

	event_flags_external_interaction.set(STOP_VIDEO_FLAG);
	for (int i = 0; i < 3; i++) {
		event_flags_external_interaction.clear(MODE_USER1_FLAG + ENABLE_MOTORS_ONE_SPIN_FLAG +
											   ENABLE_MOTORS_ALT_SPIN_FLAG);
		rtos::ThisThread::sleep_for(500ms);
	}
	rtos::ThisThread::sleep_for(500ms);
	display_utils.setOff();
	leds_utils.turnOff(LedsRange::all);

	rtos::ThisThread::sleep_for(100ms);
}

void pingRobot()
{
	turnOffAllActuators();

	for (int i = 0; i < 3; i++) {
		leds_utils.turnOn(LedsRange::belt, CRGB::RoyalBlue);
		rtos::ThisThread::sleep_for(400ms);
		leds_utils.turnOff(LedsRange::all);
		rtos::ThisThread::sleep_for(400ms);
	}
}

void enableVideo()
{
	display_utils.displayVideo("animation-face-state-waiting");
	event_flags_external_interaction.set(START_VIDEO_FLAG);
}

void motorsLoop()
{
	auto enable_motors_one_spin_flag_is_set = []() {
		return (event_flags_external_interaction.get() & ENABLE_MOTORS_ONE_SPIN_FLAG) == ENABLE_MOTORS_ONE_SPIN_FLAG;
	};
	auto enable_motors_alt_spin_flag_is_set = []() {
		return (event_flags_external_interaction.get() & ENABLE_MOTORS_ALT_SPIN_FLAG) == ENABLE_MOTORS_ALT_SPIN_FLAG;
	};

	motors_utils.setSpeed(1.0F, 1.0F);

	while (true) {
		event_flags_external_interaction.clear(ENABLE_MOTORS_ONE_SPIN_FLAG + ENABLE_MOTORS_ALT_SPIN_FLAG);
		event_flags_external_interaction.wait_any(ENABLE_MOTORS_ONE_SPIN_FLAG + ENABLE_MOTORS_ALT_SPIN_FLAG,
												  osWaitForever, false);
		if (enable_motors_one_spin_flag_is_set()) {
			motors_utils.spin(MotorsUtils::SpinDirection::Left);
		} else if (enable_motors_alt_spin_flag_is_set()) {
			while (enable_motors_alt_spin_flag_is_set()) {
				motors_utils.spin(MotorsUtils::SpinDirection::Left);
				rtos::ThisThread::sleep_for(1s);
				if (!enable_motors_alt_spin_flag_is_set()) {
					continue;
				}
				motors_utils.spin(MotorsUtils::SpinDirection::Right);
				rtos::ThisThread::sleep_for(1s);
			}
			motors_utils.stop();
		}
	}
}

void rfidLoop()
{
	event_flags_external_interaction.wait_any(ENABLE_RFID_FLAG);

	auto rfid_utils = RFIDUtils {event_flags_external_interaction};

	rfid_utils.initialize();
	rfid_utils.registerEventQueue(event_queue);

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}

void wifiLoop()
{
	auto enable_wifi_download_flag_is_set = []() {
		return (event_flags_external_interaction.get() & ENABLE_WIFI_FLAG) == ENABLE_WIFI_FLAG;
	};

	event_flags_external_interaction.wait_any(ENABLE_WIFI_FLAG, osWaitForever, false);

	auto wifi_utils = WifiUtils {};

	while (true) {
		event_flags_external_interaction.wait_any(ENABLE_WIFI_FLAG, osWaitForever, false);
		if (wifi_utils.connect(wifi_ssid.data(), wifi_pass.data())) {
			while (enable_wifi_download_flag_is_set()) {
				wifi_utils.downloadFile("https://github.com/leka/LekaOS/releases/download/1.2.3/LekaOS-1.2.3.bin");

				// rtos::ThisThread::sleep_for(1s);
			}
			wifi_utils.disconnect();
		} else {
			event_flags_external_interaction.clear(ENABLE_WIFI_FLAG);
		}
	}
}

auto ping_flag_is_set = []() { return (event_flags_external_interaction.get() & BLE_PING_FLAG) == BLE_PING_FLAG; };
auto reboot_instruction_flag_is_set = []() {
	return (event_flags_external_interaction.get() & BLE_REBOOT_INSTRUCTION_FLAG) == BLE_REBOOT_INSTRUCTION_FLAG;
};

auto wifi_ssid_flag_is_set = []() { return (event_flags_external_interaction.get() & WIFI_SSID) == WIFI_SSID; };
auto wifi_pass_flag_is_set = []() { return (event_flags_external_interaction.get() & WIFI_PASS) == WIFI_PASS; };

auto enable_screen_flag_is_set = []() {
	return (event_flags_external_interaction.get() & ENABLE_SCREEN_FLAG) == ENABLE_SCREEN_FLAG;
};

auto main() -> int
{
	// COMMON INITILIZATION
	startWatchdog(thread_watchdog);

	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	thread_event_queue.start({&event_queue, &events::EventQueue::dispatch_forever});

	log_info("Hello, World!\n\n");
	hello.start();

	leds_utils.initialize();
	motors_utils.stop();

	battery_utils.registerEventQueue(event_queue);

	ble_utils.setDeviceName("LekaRadioCEM");
	thread_ble.start({&ble_utils, &BLEUtils::startAdvertising});
	event_flags_external_interaction.set(ENABLE_BLE_FLAG);
	auto update_battery_level = [&]() { ble_utils.setBatteryLevel(battery_utils.getBatteryLevel()); };
	event_queue.call_every(1s, update_battery_level);

	display_utils.initializeSD();
	display_utils.initializeScreen();

	thread_motors.start(motorsLoop);

	thread_rfid.start(rfidLoop);

	thread_wifi.start(wifiLoop);

	// ON EXIT INITIALIZATION
	thread_leds.start(ledsLoop);
	leds_utils.initializationAnimation();
	leds_utils.setBrightness(0xFF);

	//

	//

	//

	turnOffAllActuators();

	while (true) {
		log_info("Still alive...");
		rtos::ThisThread::sleep_for(100ms);

		event_flags_external_interaction.clear(BLE_PING_FLAG | BLE_REBOOT_INSTRUCTION_FLAG | BLE_MODE_FLAG | WIFI_SSID |
											   WIFI_PASS);
		event_flags_external_interaction.wait_any(NEW_BLE_MESSAGE_FLAG);

		if (ping_flag_is_set()) {
			pingRobot();
		}
		if (reboot_instruction_flag_is_set()) {
			NVIC_SystemReset();
		}
		if (wifi_ssid_flag_is_set()) {
			wifi_ssid = ble_utils.getWifiSSID();
		}
		if (wifi_pass_flag_is_set()) {
			wifi_pass = ble_utils.getWifiPass();
		}

		auto currentMode = ble_utils.getMode();
		if (battery_utils.isInCharge()) {
			if (currentMode == 0x11) {
				enableVideo();
				event_flags_external_interaction.set(ENABLE_WIFI_FLAG);
			} else if (currentMode == 0x91) {
				event_flags_external_interaction.set(MODE_USER1_FLAG);
				event_flags_external_interaction.set(ENABLE_WIFI_FLAG);
			} else if (currentMode == 0x92) {
				event_flags_external_interaction.set(MODE_USER1_FLAG + ENABLE_MOTORS_ONE_SPIN_FLAG);
				event_flags_external_interaction.set(ENABLE_WIFI_FLAG);
			} else if (currentMode == 0x92) {
				event_flags_external_interaction.set(MODE_USER1_FLAG + ENABLE_MOTORS_ALT_SPIN_FLAG);
				event_flags_external_interaction.set(ENABLE_WIFI_FLAG);
			} else {
				event_flags_external_interaction.clear(ENABLE_WIFI_FLAG);
				turnOffAllActuators();
			}

		} else {
			battery_utils.setUserMode();
			event_flags_external_interaction.clear(ENABLE_WIFI_FLAG);

			if (currentMode == 0x01) {
				event_flags_external_interaction.set(MODE_USER1_FLAG);
			} else if (currentMode == 0x02) {
				event_flags_external_interaction.set(MODE_USER1_FLAG + ENABLE_MOTORS_ONE_SPIN_FLAG);
			} else if (currentMode == 0x03) {
				event_flags_external_interaction.set(MODE_USER1_FLAG + ENABLE_MOTORS_ALT_SPIN_FLAG);
			} else if (currentMode == 0x21) {
				event_flags_external_interaction.set(ENABLE_RFID_FLAG);
				ble_utils.shutdown();
			} else if (currentMode == 0x37) {
				ble_utils.stopAdvertising();
				ble_utils.setChannel(PrimaryChannel::channel37);
				event_flags_external_interaction.set(ENABLE_BLE_FLAG);
			} else if (currentMode == 0x38) {
				ble_utils.stopAdvertising();
				ble_utils.setChannel(PrimaryChannel::channel38);
				event_flags_external_interaction.set(ENABLE_BLE_FLAG);
			} else if (currentMode == 0x39) {
				ble_utils.stopAdvertising();
				ble_utils.setChannel(PrimaryChannel::channel39);
				event_flags_external_interaction.set(ENABLE_BLE_FLAG);
			} else if (currentMode == 0x41) {
				event_flags_external_interaction.set(ENABLE_WIFI_FLAG);
				ble_utils.shutdown();
			} else if (currentMode == 0x91) {
				event_flags_external_interaction.set(MODE_USER1_FLAG);
				event_flags_external_interaction.set(ENABLE_WIFI_FLAG);
			} else if (currentMode == 0x92) {
				event_flags_external_interaction.set(MODE_USER1_FLAG + ENABLE_MOTORS_ONE_SPIN_FLAG);
				event_flags_external_interaction.set(ENABLE_WIFI_FLAG);
			} else if (currentMode == 0x92) {
				event_flags_external_interaction.set(MODE_USER1_FLAG + ENABLE_MOTORS_ALT_SPIN_FLAG);
				event_flags_external_interaction.set(ENABLE_WIFI_FLAG);
			} else {
				turnOffAllActuators();
			}

			if (enable_screen_flag_is_set()) {
				enableVideo();
			}
		}
	}
}
