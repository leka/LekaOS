// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>
#include <cstdint>

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreBattery.h"
#include "CoreMotor.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "Utils.h"

using namespace leka;
using namespace std::chrono;

constexpr auto PRINT_BATTERY_VOLTAGE_LOG = false;
constexpr auto fileName =
	std::array<char, 36> {"2021_03_30-Battery-Voltage-Log.csv"};   // YYYY_MM_DD-Battery-Voltage-Log.csv

const FileManager sd;

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	HelloWorld hello;
	hello.start();

	auto battery = CoreBattery {PinName::BATTERY_VOLTAGE};

	auto battery_thread = rtos::Thread {};
	auto battery_lambda = [&battery] {
		auto now	 = [] { return static_cast<int>(rtos::Kernel::Clock::now().time_since_epoch().count()); };
		auto voltage = [&] { return battery.readVoltage(); };

		auto buffer = std::array<char, 64> {};

		while (true) {
			auto length = snprintf(buffer.data(), std::size(buffer), "%i, %f\n", now(), voltage());
			writeSDFile(fileName.data(), buffer.data(), length);
			rtos::ThisThread::sleep_for(30s);
		}
	};

	auto motor_right = CoreMotor {MOTOR_RIGHT_DIRECTION_1, MOTOR_RIGHT_DIRECTION_2, MOTOR_RIGHT_PWM};
	auto motor_left	 = CoreMotor {MOTOR_LEFT_DIRECTION_1, MOTOR_LEFT_DIRECTION_2, MOTOR_LEFT_PWM};

	auto motors_thread = rtos::Thread {};
	auto motors_lambda = [&motor_left, &motor_right] {
		while (true) {
			motor_left.spin(Rotation::counterClockwise, 1.0f);
			motor_right.spin(Rotation::counterClockwise, 1.0f);

			rtos::ThisThread::sleep_for(50s);

			motor_left.stop();
			motor_right.stop();

			rtos::ThisThread::sleep_for(10s);
		}
	};

	rtos::ThisThread::sleep_for(2s);

	log_info("Starting...");

	rtos::ThisThread::sleep_for(1s);

	if (PRINT_BATTERY_VOLTAGE_LOG) {
		auto buffer = std::array<char, 2048> {};
		auto length = readSdFile(fileName.data(), buffer.data(), std::size(buffer));

		log_info("Battery data:\n");
		serial.write(buffer.data(), length);
	} else {
		battery_thread.start(battery_lambda);
		motors_thread.start(motors_lambda);
	}

	while (true) {
		log_info("Main thread is running...");
		rtos::ThisThread::sleep_for(5s);
	}
}
