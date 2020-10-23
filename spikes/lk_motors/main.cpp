// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

#include "PinNames.h"

#include "HelloWorld.h"
#include "Motor.h"

using namespace leka;

HelloWorld hello;

DigitalOut motor_right_dir_1(PE_4);
DigitalOut motor_right_dir_2(PE_5);
PwmOut motor_right_speed(PA_2);

DigitalOut motor_left_dir_1(PC_13);
DigitalOut motor_left_dir_2(PI_8);
PwmOut motor_left_speed(PA_1);

Motor motor_right(motor_right_dir_1, motor_right_dir_2, motor_right_speed);
Motor motor_left(motor_left_dir_1, motor_left_dir_2, motor_left_speed);

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
