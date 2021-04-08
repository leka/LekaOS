// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "./Utils.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LKCoreBattery.h"
#include "LKCoreMotor.h"
#include "SDBlockDevice.h"

// Setup important variables
#define PRINT_BATTERY_VOLTAGE_LOG false									 // True to read a file, false to enable log
#define BATTERY_LOG_FILENAME	  "2021_03_30-Battery-Voltage-Log.csv"	 // YYYY_MM_DD-Battery-Voltage-Log.csv

using namespace leka;

HelloWorld hello;
FileManager sd;

LKCoreBattery battery(PinName::BATTERY_VOLTAGE);

LKCoreMotor motor_right(MOTOR_RIGHT_DIRECTION_1, MOTOR_RIGHT_DIRECTION_2, MOTOR_RIGHT_PWM);
LKCoreMotor motor_left(MOTOR_LEFT_DIRECTION_1, MOTOR_LEFT_DIRECTION_2, MOTOR_LEFT_PWM);

const char fileName[] = BATTERY_LOG_FILENAME;

Thread battery_logger_thread;
Thread motor_thread;

void batteryLoggerThread()
{
	char buffer[64] {0};

	auto now	 = [] { return static_cast<int>(Kernel::Clock::now().time_since_epoch().count()); };
	auto voltage = [] { return battery.readVoltage(); };

	while (true) {
		auto length = sprintf(buffer, "%i, %f\n", now(), voltage());
		writeSDFile(fileName, buffer, length);

		rtos::ThisThread::sleep_for(30s);
	}
}

void motorThread()
{
	while (true) {
		motor_right.spin(Rotation::counterClockwise, 1.0f);
		motor_left.spin(Rotation::counterClockwise, 1.0f);

		rtos::ThisThread::sleep_for(50s);

		motor_right.stop();
		motor_left.stop();

		rtos::ThisThread::sleep_for(10s);
	}
}

int main(void)
{
	rtos::ThisThread::sleep_for(2s);

	hello.start();

	rtos::ThisThread::sleep_for(2s);

	if (PRINT_BATTERY_VOLTAGE_LOG) {
		constexpr uint16_t buffeur_size = 2048;
		char buffer[buffeur_size];

		auto length = readSdFile(fileName, buffer, buffeur_size);
		for (auto i = 0; i < length; ++i) {
			printf("%c", buffer[i]);
		}
	} else {
		battery_logger_thread.start(batteryLoggerThread);
		motor_thread.start(motorThread);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10s);
	}
}
