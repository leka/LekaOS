#include "MotorsUtils.h"

using namespace leka;

void MotorsUtils::setSpeed(float left_speed, float right_speed)
{
	_motor_left_speed.write(left_speed);
	_motor_right_speed.write(right_speed);

	left_speed	 = left_speed;
	_right_speed = right_speed;
}

void MotorsUtils::spin(SpinDirection direction)
{
	auto motor_rotation = Rotation {direction};

	// _motor_left.spin(motor_rotation, _motor_left_speed.read());
	// _motor_right.spin(motor_rotation, _motor_right_speed.read());
	_motor_left.spin(motor_rotation, _left_speed);
	_motor_right.spin(motor_rotation, _right_speed);
}

void MotorsUtils::stop()
{
	_motor_left.stop();
	_motor_right.stop();
}
