// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_MOTORS_UTILS_H_
#define _LEKA_OS_SPIKE_MOTORS_UTILS_H_

#include "PinNames.h"

#include "LekaMotors.h"
#include "Motor.h"

DigitalOut motor_right_dir_1(MOTOR_RIGHT_DIRECTION_1);
DigitalOut motor_right_dir_2(MOTOR_RIGHT_DIRECTION_2);
PwmOut motor_right_speed(MOTOR_RIGHT_PWM);

DigitalOut motor_left_dir_1(MOTOR_LEFT_DIRECTION_2);
DigitalOut motor_left_dir_2(MOTOR_LEFT_DIRECTION_1);
PwmOut motor_left_speed(MOTOR_LEFT_PWM);

Motor motor_right(motor_right_dir_1, motor_right_dir_2, motor_right_speed);
Motor motor_left(motor_left_dir_1, motor_left_dir_2, motor_left_speed);

Motors motors(motor_right, motor_left);

uint8_t speed = 0xFF;

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
