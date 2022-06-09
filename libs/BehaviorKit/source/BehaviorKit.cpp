// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "rtos/ThisThread.h"

namespace leka {

using namespace std::chrono;

void BehaviorKit::spinLeft(float speed)
{
	_motor_left.spin(Rotation::clockwise, speed);
	_motor_right.spin(Rotation::clockwise, speed);
}

void BehaviorKit::spinRight(float speed)
{
	_motor_left.spin(Rotation::counterClockwise, speed);
	_motor_right.spin(Rotation::counterClockwise, speed);
}

void BehaviorKit::launching()
{
	_videokit.displayImage("fs/home/img/robot-misc-splash_screen-large-400-004C.jpg");
}

void BehaviorKit::sleeping()
{
	_ledkit.start(&LedKit::animation::sleeping);
	_videokit.playVideoOnce("/fs/home/vid/animation-face-state-yawning-sleeping_without_eyebrows.avi");
}

void BehaviorKit::waiting()
{
	_ledkit.stop();
	_videokit.playVideoOnRepeat(
		"/fs/home/vid/animation-face-state-waiting-looking-top-right-to-left-without-eyebrows.avi");
}

void BehaviorKit::lowBattery()
{
	_ledkit.stop();
	_videokit.displayImage("fs/home/img/robot-battery-empty-must_be_charged-004F.jpg");
}

void BehaviorKit::chargingZero()
{
	_videokit.displayImage("fs/home/img/robot-battery-charging-empty_red-0082.jpg");
}

void BehaviorKit::chargingRed()
{
	_videokit.displayImage("fs/home/img/robot-battery-charging-low_red-0050.jpg");
}

void BehaviorKit::chargingOrange()
{
	_videokit.displayImage("fs/home/img/robot-battery-charging-medium_yellow-0051.jpg");
}

void BehaviorKit::chargingYellow()
{
	_videokit.displayImage("fs/home/img/robot-battery-charging-high_green-0052.jpg");
}

void BehaviorKit::chargingGreen()
{
	_videokit.displayImage("fs/home/img/robot-battery-charging-full_green-0053.jpg");
}

void BehaviorKit::blinkGreen()
{
	spinLeft(1);
	_ledkit.start(&LedKit::animation::blink_green);
	_videokit.playVideoOnce("/fs/home/vid/animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::spinBlink()
{
	spinRight(1);
	_ledkit.start(&LedKit::animation::spin_blink);
	_videokit.playVideoOnce("/fs/home/vid/animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::fire()
{
	_ledkit.start(&LedKit::animation::fire);
	_videokit.playVideoOnce("/fs/home/vid/animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
}

void BehaviorKit::sprinkles()
{
	_ledkit.start(&LedKit::animation::sprinkles);
	_videokit.playVideoOnce("/fs/home/vid/animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
}

void BehaviorKit::rainbow()
{
	_ledkit.start(&LedKit::animation::rainbow);
	_videokit.playVideoOnce("/fs/home/vid/animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(3s);
	_ledkit.stop();
}

void BehaviorKit::bleConnection(bool with_video)
{
	_ledkit.start(&LedKit::animation::ble_connection);

	if (with_video) {
		_videokit.playVideoOnce("/fs/home/vid/animation-face-action-wink-without-eyebrows.avi");
	}
}

void BehaviorKit::working()
{
	_videokit.displayImage("fs/home/img/robot-face-smiling-slightly-003F.jpg");
}

void BehaviorKit::stop()
{
	_ledkit.stop();
	_videokit.stopVideo();
	_motor_left.stop();
	_motor_right.stop();
}

}	// namespace leka
