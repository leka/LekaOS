// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "drivers/BufferedSerial.h"
#include "drivers/PwmOut.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreMotor.h"
#include "CoreMotorBase.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

void spinLeft(CoreMotorBase &left, CoreMotorBase &right)
{
	left.spin(Rotation::clockwise, 0.5f);
	right.spin(Rotation::clockwise, 0.5f);
}

void spinRight(CoreMotorBase &left, CoreMotorBase &right)
{
	left.spin(Rotation::counterClockwise, 0.5f);
	right.spin(Rotation::counterClockwise, 0.5f);
}

void stop(CoreMotorBase &left, CoreMotorBase &right)
{
	left.stop();
	right.stop();
}

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	CoreMotor motor_right(MOTOR_RIGHT_DIRECTION_1, MOTOR_RIGHT_DIRECTION_2, MOTOR_RIGHT_PWM);
	CoreMotor motor_left(MOTOR_LEFT_DIRECTION_1, MOTOR_LEFT_DIRECTION_2, MOTOR_LEFT_PWM);

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);

		log_info("Spin left...");
		spinLeft(motor_left, motor_right);

		rtos::ThisThread::sleep_for(5s);

		log_info("Spin right...");
		spinRight(motor_left, motor_right);

		rtos::ThisThread::sleep_for(5s);

		log_info("Spin stop...");
		stop(motor_left, motor_right);

		rtos::ThisThread::sleep_for(5s);
	}
}
