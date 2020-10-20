// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_MOTORS_UTILS_H_
#define _LEKA_OS_SPIKE_MOTORS_UTILS_H_

#include "LekaMotors.h"
#include "Motor.h"

DigitalOut motor_right_dir_1(PE_4);
DigitalOut motor_right_dir_2(PE_5);
PwmOut motor_right_speed(PA_2);

DigitalOut motor_left_dir_1(PC_13);
DigitalOut motor_left_dir_2(PI_8);
PwmOut motor_left_speed(PA_1);

Motor motor_right(motor_right_dir_1, motor_right_dir_2, motor_right_speed);
Motor motor_left(motor_left_dir_1, motor_left_dir_2, motor_left_speed);

Motors motors(motor_right, motor_left);

void motor_thread()
{
	float speed = 0.6;
	while (true) {
		printf("motor spin left\n");
		motors.spin(Turn::left, speed);
		// spinLeft();
		rtos::ThisThread::sleep_for(5s);

		printf("motors stop\n");
		motors.stop();
		rtos::ThisThread::sleep_for(5s);

		printf("turn right\n");
		motors.spin(Turn::right, speed);
		// spinRight();
		rtos::ThisThread::sleep_for(5s);

		printf("motor brakes\n");
		motors.brake();
		// stop();
		rtos::ThisThread::sleep_for(5s);
	}
}

#endif	 // _LEKA_OS_SPIKE_MOTORS_UTILS_H_
