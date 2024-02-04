// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "HappyToupie.h"

#include "rtos/ThisThread.h"

#include "MathUtils.h"

using namespace leka;
using namespace std::chrono;

auto HappyToupie::convertToPwmFrom(float angle) const -> float
{
	auto res = utils::math::map(angle, kMinAngleInput, kMaxAngleInput, kMinPwmOutput, kMaxPwmOutput);
	return res;
}

void HappyToupie::start()
{
	auto on_timeout = [this] { stop(); };
	_timeout.onTimeout(on_timeout);
	_timeout.start(20s);

	should_stop = false;

	_event_loop.registerCallback([this] { run(); });
	_event_loop.start();
}

void HappyToupie::stop()
{
	should_stop = true;
	_event_loop.stop();
	rtos::ThisThread::sleep_for(100ms);
	stopMotors();
	_timeout.stop();
}

void HappyToupie::run()
{
	if (should_stop || _battery.isCharging()) {
		stop();
		return;
	}

	spinLeft(1.F, 1.F);

	rtos::ThisThread::sleep_for(10ms);
	_event_loop.start();
}

void HappyToupie::stopMotors()
{
	_motor_right.stop();
	_motor_left.stop();
}

void HappyToupie::spinLeft(float left_speed, float right_speed)
{
	_motor_left.spin(Rotation::clockwise, left_speed);
	_motor_right.spin(Rotation::clockwise, right_speed);
}

void HappyToupie::spinRight(float left_speed, float right_speed)
{
	_motor_left.spin(Rotation::counterClockwise, left_speed);
	_motor_right.spin(Rotation::counterClockwise, right_speed);
}

void HappyToupie::moveForward(float speed)
{
	_motor_left.spin(Rotation::counterClockwise, speed);
	_motor_right.spin(Rotation::clockwise, speed);
}

void HappyToupie::moveBackward(float speed)
{
	_motor_left.spin(Rotation::clockwise, speed);
	_motor_right.spin(Rotation::counterClockwise, speed);
}
