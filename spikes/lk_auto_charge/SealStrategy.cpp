// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "SealStrategy.h"

#include "rtos/ThisThread.h"

#include "MathUtils.h"

using namespace leka;
using namespace std::chrono;

auto SealStrategy::convertToPwmFrom(float angle) const -> float
{
	auto res = utils::math::map(angle, kMinAngleInput, kMaxAngleInput, kMinPwmOutput, kMaxPwmOutput);
	return res;
}

void SealStrategy::start()
{
	auto on_timeout = [this] { stop(); };
	_timeout.onTimeout(on_timeout);
	_timeout.start(20s);

	should_stop = false;

	_event_loop.registerCallback([this] { run(); });
	_event_loop.start();
}

void SealStrategy::stop()
{
	should_stop = true;
	_event_loop.stop();
	rtos::ThisThread::sleep_for(100ms);
	stopMotors();
	_timeout.stop();
}

void SealStrategy::run()
{
	if (should_stop || _battery.isCharging()) {
		stop();
		return;
	}

	auto angles				 = _imukit.getEulerAngles();
	auto is_right_tilted	 = angles.roll > 0;
	auto should_move_forward = angles.pitch > 0;

	auto abs_float = [](float value) { return value > 0 ? value : -value; };

	if (abs_float(angles.roll) > kRollTolerance) {
		auto speed_offset = convertToPwmFrom(angles.roll > 0 ? angles.roll : -angles.roll);
		if (is_right_tilted) {
			if (should_move_forward) {
				spinRight(kMinPwmOutput, kMinPwmOutput + speed_offset);
			} else {
				spinLeft(kMinPwmOutput, kMinPwmOutput + speed_offset);
			}
		} else {
			if (should_move_forward) {
				spinLeft(kMinPwmOutput + speed_offset, kMinPwmOutput);
			} else {
				spinRight(kMinPwmOutput + speed_offset, kMinPwmOutput);
			}
		}
	} else if (abs_float(angles.pitch) > kPitchTolerance) {
		auto speed = convertToPwmFrom(angles.pitch > 0 ? angles.pitch : -angles.pitch);
		if (should_move_forward) {
			moveForward(speed);
		} else {
			moveBackward(speed);
		}
	} else {
		stopMotors();
	}

	rtos::ThisThread::sleep_for(10ms);
	_event_loop.start();
}

void SealStrategy::stopMotors()
{
	_motor_right.stop();
	_motor_left.stop();
}

void SealStrategy::spinLeft(float left_speed, float right_speed)
{
	_motor_left.spin(Rotation::clockwise, left_speed);
	_motor_right.spin(Rotation::clockwise, right_speed);
}

void SealStrategy::spinRight(float left_speed, float right_speed)
{
	_motor_left.spin(Rotation::counterClockwise, left_speed);
	_motor_right.spin(Rotation::counterClockwise, right_speed);
}

void SealStrategy::moveForward(float speed)
{
	_motor_left.spin(Rotation::counterClockwise, speed);
	_motor_right.spin(Rotation::clockwise, speed);
}

void SealStrategy::moveBackward(float speed)
{
	_motor_left.spin(Rotation::clockwise, speed);
	_motor_right.spin(Rotation::counterClockwise, speed);
}
