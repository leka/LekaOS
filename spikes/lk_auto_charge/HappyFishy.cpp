// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "HappyFishy.h"

#include "rtos/ThisThread.h"

#include "MathUtils.h"

using namespace leka;
using namespace std::chrono;

auto HappyFishy::convertToPwmFrom(float angle) const -> float
{
	auto res = utils::math::map(angle, kMinAngleInput, kMaxAngleInput, kMinPwmOutput, kMaxPwmOutput);
	return res;
}

void HappyFishy::start()
{
	auto on_timeout = [this] { stop(); };
	_timeout.onTimeout(on_timeout);
	_timeout.start(20s);

	should_stop = false;

	_event_loop.registerCallback([this] { run(); });
	_event_loop.start();
}

void HappyFishy::stop()
{
	should_stop = true;
	_event_loop.stop();
	rtos::ThisThread::sleep_for(100ms);
	stopMotors();
	_timeout.stop();
}

void HappyFishy::run()
{
	if (should_stop || _battery.isCharging()) {
		stop();
		return;
	}

	if (move_left) {
		spinLeft(0.25F, 0.25F);
		rtos::ThisThread::sleep_for(300ms);
	} else {
		spinRight(0.25F, 0.25F);
		rtos::ThisThread::sleep_for(320ms);
	}
	move_left = !move_left;

	_event_loop.start();
}

void HappyFishy::stopMotors()
{
	_motor_right.stop();
	_motor_left.stop();
}

void HappyFishy::spinLeft(float left_speed, float right_speed)
{
	_motor_left.spin(Rotation::clockwise, left_speed);
	_motor_right.spin(Rotation::clockwise, right_speed);
}

void HappyFishy::spinRight(float left_speed, float right_speed)
{
	_motor_left.spin(Rotation::counterClockwise, left_speed);
	_motor_right.spin(Rotation::counterClockwise, right_speed);
}

void HappyFishy::moveForward(float speed)
{
	_motor_left.spin(Rotation::counterClockwise, speed);
	_motor_right.spin(Rotation::clockwise, speed);
}

void HappyFishy::moveBackward(float speed)
{
	_motor_left.spin(Rotation::clockwise, speed);
	_motor_right.spin(Rotation::counterClockwise, speed);
}
