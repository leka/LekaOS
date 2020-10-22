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

float h		  = 120.0;
uint8_t speed = h;

void motor_thread()
{
	while (true) {
		motors.move(Direction::forward, speed);
		rtos::ThisThread::sleep_for(6s);

		motors.stop();
		rtos::ThisThread::sleep_for(3s);

		motors.spin(Rotation::right, speed);
		rtos::ThisThread::sleep_for(1s);

		motors.stop();
		rtos::ThisThread::sleep_for(3s);

		motors.move(Direction::forward, speed);
		rtos::ThisThread::sleep_for(6s);

		motors.stop();
		rtos::ThisThread::sleep_for(60s);
	}
}

#endif	 // _LEKA_OS_SPIKE_MOTORS_UTILS_H_
