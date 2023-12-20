// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "behaviors/AutochargeSeal.h"

#include "rtos/ThisThread.h"

#include "MathUtils.h"

using namespace leka::behavior;
using namespace std::chrono;

auto AutochargeSeal::id() -> BehaviorID
{
	return _id;
}

auto AutochargeSeal::convertToPwmFrom(float angle) const -> float
{
	auto res = utils::math::map(angle, kMinAngleInput, kMaxAngleInput, kMinPwmOutput, kMaxPwmOutput);
	return res;
}

void AutochargeSeal::run()
{
	must_stop = true;	// TODO: Update for UT

	do {
		loop();
	} while (!must_stop);
}

void AutochargeSeal::loop()
{
	auto angles				 = _imukit.getEulerAngles();
	auto is_right_tilted	 = angles.roll > 0;
	auto should_move_forward = angles.pitch > 0;

	auto abs_float = [](float value) { return value > 0 ? value : -value; };

	if (abs_float(angles.roll) > kRollTolerance) {
		auto speed_offset = convertToPwmFrom(angles.roll > 0 ? angles.roll : -angles.roll);
		spinToFixRoll(is_right_tilted, should_move_forward, speed_offset);
	} else if (abs_float(angles.pitch) > kPitchTolerance) {
		auto speed = convertToPwmFrom(angles.pitch > 0 ? angles.pitch : -angles.pitch);
		moveToFixPitch(should_move_forward, speed);
	} else {
		stopMotors();
	}

	rtos::ThisThread::sleep_for(10ms);
}

void AutochargeSeal::stop()
{
	must_stop = true;
	rtos::ThisThread::sleep_for(100ms);
	stopMotors();
}

void AutochargeSeal::stopMotors()
{
	_motor_right.stop();
	_motor_left.stop();
}

void AutochargeSeal::spinLeft(float left_speed, float right_speed)
{
	_motor_left.spin(Rotation::clockwise, left_speed);
	_motor_right.spin(Rotation::clockwise, right_speed);
}

void AutochargeSeal::spinRight(float left_speed, float right_speed)
{
	_motor_left.spin(Rotation::counterClockwise, left_speed);
	_motor_right.spin(Rotation::counterClockwise, right_speed);
}

void AutochargeSeal::moveForward(float speed)
{
	_motor_left.spin(Rotation::counterClockwise, speed);
	_motor_right.spin(Rotation::clockwise, speed);
}

void AutochargeSeal::moveBackward(float speed)
{
	_motor_left.spin(Rotation::clockwise, speed);
	_motor_right.spin(Rotation::counterClockwise, speed);
}

void AutochargeSeal::spinToFixRoll(bool is_right_tilted, bool should_move_forward, float speed_offset)
{
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
}

void AutochargeSeal::moveToFixPitch(bool should_move_forward, float speed)
{
	if (should_move_forward) {
		moveForward(speed);
	} else {
		moveBackward(speed);
	}
}
