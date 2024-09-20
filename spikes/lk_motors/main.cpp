// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/DigitalOut.h"
#include "drivers/PwmOut.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "AnalogIn.h"
#include "CoreMotor.h"
#include "CorePwm.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

void spinLeft(interface::Motor &left, interface::Motor &right)
{
	left.spin(Rotation::clockwise, 0.5F);
	right.spin(Rotation::clockwise, 0.5F);
}

void spinRight(interface::Motor &left, interface::Motor &right)
{
	left.spin(Rotation::counterClockwise, 0.5F);
	right.spin(Rotation::counterClockwise, 0.5F);
}

void stop(interface::Motor &left, interface::Motor &right)
{
	left.stop();
	right.stop();
}

auto main() -> int
{
	logger::init();

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	auto motor_left_dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
	auto motor_left_dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
	auto motor_left_speed = CorePwm {MOTOR_LEFT_PWM};

	auto motor_right_dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
	auto motor_right_dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
	auto motor_right_speed = CorePwm {MOTOR_RIGHT_PWM};

	auto motor_left	 = CoreMotor {motor_left_dir_1, motor_left_dir_2, motor_left_speed};
	auto motor_right = CoreMotor {motor_right_dir_1, motor_right_dir_2, motor_right_speed};

	const auto maxVSense	= float {2.3};										// min -1V
	auto motor_left_voltage = mbed::AnalogIn {MOTOR_LEFT_VOLTAGE, maxVSense};	// U = RI, R = 0.5

	auto log_voltage = [&motor_left_voltage] {
		for (int i = 0; i < 20; i++) {
			log_info("Raw: %d | Ratio: %.2f | Voltage: %.2fV | Current: %.2fA", motor_left_voltage.read_u16(),
					 motor_left_voltage.read(), motor_left_voltage.read_voltage(),
					 motor_left_voltage.read_voltage() * 2.0F);
			rtos::ThisThread::sleep_for(100ms);
		}
	};

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);

		log_info("Spin left...");
		spinLeft(motor_left, motor_right);

		log_voltage();

		log_info("Spin stop...");
		stop(motor_left, motor_right);

		log_voltage();
	}
}
