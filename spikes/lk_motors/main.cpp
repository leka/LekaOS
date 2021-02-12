// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "HelloWorld.h"
#include "LKCoreMotor.h"

using namespace leka;

HelloWorld hello;

LKCoreMotor motor_right(MOTOR_RIGHT_DIRECTION_1, MOTOR_RIGHT_DIRECTION_2, MOTOR_RIGHT_PWM);
LKCoreMotor motor_left(MOTOR_LEFT_DIRECTION_1, MOTOR_LEFT_DIRECTION_2, MOTOR_LEFT_PWM);

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

void spinLeft()
{
	motor_right.spin(Rotation::clockwise, 0.5f);
	motor_left.spin(Rotation::clockwise, 0.5f);
}

void spinRight()
{
	motor_right.spin(Rotation::counterClockwise, 0.5f);
	motor_left.spin(Rotation::counterClockwise, 0.5f);
}

void stop()
{
	motor_right.stop();
	motor_left.stop();
}

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Investigation Day!\n\n");

	rtos::ThisThread::sleep_for(2s);

	hello.start();

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);

		spinLeft();

		rtos::ThisThread::sleep_for(5s);

		spinRight();

		rtos::ThisThread::sleep_for(5s);

		stop();

		rtos::ThisThread::sleep_for(5s);
	}

	return 0;
}
