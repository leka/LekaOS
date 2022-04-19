// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "PinNames.h"

#include "CoreMotor.h"
#include "CorePwm.h"
#include "LekaMotors.h"

using namespace leka;

auto motor_left_dir_1 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_1};
auto morot_left_dir_2 = mbed::DigitalOut {MOTOR_LEFT_DIRECTION_2};
auto morot_left_speed = CorePwm {MOTOR_LEFT_PWM};

auto motor_right_dir_1 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_1};
auto morot_right_dir_2 = mbed::DigitalOut {MOTOR_RIGHT_DIRECTION_2};
auto morot_right_speed = CorePwm {MOTOR_RIGHT_PWM};

auto motor_left	 = CoreMotor {motor_left_dir_1, morot_left_dir_2, morot_left_speed};
auto motor_right = CoreMotor {motor_right_dir_1, morot_right_dir_2, morot_right_speed};

Motors motors(motor_right, motor_left);

inline constexpr uint8_t MAX_SPEED = 0xFF;

void turnForwardLeft()
{
	motor_right.spin(Rotation::clockwise, 1.0f);
	motor_left.spin(Rotation::counterClockwise, 0.8f);
}

void turnForwardRight()
{
	motor_right.spin(Rotation::clockwise, 0.8f);
	motor_left.spin(Rotation::counterClockwise, 1.0f);
}

void moveForward(float speed)
{
	motor_right.spin(Rotation::clockwise, speed);
	motor_left.spin(Rotation::counterClockwise, speed);
}

void motor_thread()
{
	while (true) {
		auto start = Kernel::Clock::now();

		while (TEST_DIFFERENT_MOVEMENTS) {
			printf("TEST_DIFFERENT_MOVEMENTS!\n");

			motors.move(Direction::forward, MAX_SPEED);
			rtos::ThisThread::sleep_for(5s);

			motors.stop();
			rtos::ThisThread::sleep_for(2s);

			if (!TEST_DIFFERENT_MOVEMENTS) break;

			motors.move(Direction::backward, MAX_SPEED);
			rtos::ThisThread::sleep_for(5s);

			motors.stop();
			rtos::ThisThread::sleep_for(2s);

			if (!TEST_DIFFERENT_MOVEMENTS) break;

			motors.spin(Rotation::right, MAX_SPEED);
			rtos::ThisThread::sleep_for(5s);

			motors.stop();
			rtos::ThisThread::sleep_for(2s);

			if (!TEST_DIFFERENT_MOVEMENTS) break;

			turnForwardRight();
			rtos::ThisThread::sleep_for(5s);

			motors.stop();
			rtos::ThisThread::sleep_for(2s);

			if (!TEST_DIFFERENT_MOVEMENTS) break;

			turnForwardLeft();
			rtos::ThisThread::sleep_for(5s);

			motors.stop();
			TEST_DIFFERENT_MOVEMENTS = false;
		}

		while (TEST_PROGRESSIVE_ACCELERATION) {
			printf("TEST_PROGRESSIVE_ACCELERATION\n");

			for (int speed = 0; speed <= 100; speed += 20) {
				moveForward(static_cast<float>(speed) / 100);
				rtos::ThisThread::sleep_for(3s);
				if (!TEST_PROGRESSIVE_ACCELERATION) {
					motors.stop();
					break;
				}
			}

			if (!TEST_PROGRESSIVE_ACCELERATION) {
				motors.stop();
				break;
			}

			for (int speed = 80; speed >= 0; speed -= 20) {
				moveForward(static_cast<float>(speed) / 100);
				rtos::ThisThread::sleep_for(3s);
				if (!TEST_PROGRESSIVE_ACCELERATION) {
					motors.stop();
					break;
				}
			}

			motors.stop();
			TEST_PROGRESSIVE_ACCELERATION = false;
		}

		auto stop	  = Kernel::Clock::now();
		auto duration = stop - start;

		if (auto count = int(duration.count()) != 0) {
			printf("%i\n", count);
		}

		rtos::ThisThread::sleep_for(1s);
	}
}
