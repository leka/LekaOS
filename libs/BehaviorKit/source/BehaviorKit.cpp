// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "rtos/ThisThread.h"

namespace leka {

using namespace std::chrono;

inline constexpr auto reinforcer_duration = 2700ms;

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
	_videokit.displayImage("/fs/images/logo.jpg");
}

void BehaviorKit::sleeping()
{
	_ledkit.start(&LedKit::animation::sleeping);
	_videokit.playVideoOnce("/fs/videos/2022_01_17-animation-face-state-yawning-sleeping_without_eyebrows.avi");
}

void BehaviorKit::waiting()
{
	_ledkit.stop();
	_videokit.playVideoOnRepeat(
		"/fs/videos/2022_02_14-animation-face-state-waiting-looking-top-right-to-left-without-eyebrows.avi");
}

void BehaviorKit::lowBattery()
{
	_ledkit.stop();
	_videokit.displayImage("/fs/images/loading.jpg");
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::chargingEmpty()
{
	_videokit.displayImage("/fs/images/low_battery.jpg");
}

void BehaviorKit::chargingLow()
{
	_videokit.displayImage("/fs/images/battery_red.jpg");
}

void BehaviorKit::chargingMedium()
{
	_videokit.displayImage("/fs/images/battery_yellow_2.jpg");
}

void BehaviorKit::chargingHigh()
{
	_videokit.displayImage("/fs/images/battery_green_3.jpg");
}

void BehaviorKit::chargingFull()
{
	_videokit.displayImage("/fs/images/battery_green_4.jpg");
}

void BehaviorKit::blinkGreen()
{
	spinLeft(1);
	_ledkit.start(&LedKit::animation::blink_green);
	_videokit.playVideoOnce("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::spinBlink()
{
	spinRight(1);
	_ledkit.start(&LedKit::animation::spin_blink);
	_videokit.playVideoOnce("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::fire()
{
	_ledkit.start(&LedKit::animation::fire);
	_videokit.playVideoOnce("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
}

void BehaviorKit::sprinkles()
{
	_ledkit.start(&LedKit::animation::sprinkles);
	_videokit.playVideoOnce("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
}

void BehaviorKit::rainbow()
{
	_ledkit.start(&LedKit::animation::rainbow);
	_videokit.playVideoOnce("/fs/videos/2022_02_14-animation-face-state-happy-without-eyebrows.avi");
	rtos::ThisThread::sleep_for(reinforcer_duration);
	_ledkit.stop();
}

void BehaviorKit::bleConnection(bool with_video)
{
	_ledkit.start(&LedKit::animation::ble_connection);
	if (with_video) {
		_videokit.playVideoOnce("/fs/videos/2022_02_14-animation-face-action-wink-without-eyebrows.avi");
	}
}

void BehaviorKit::working()
{
	_videokit.displayImage("/fs/images/robot-emotion-neutral.jpg");
}

void BehaviorKit::stop()
{
	_ledkit.stop();
	_videokit.stopVideo();
	_motor_left.stop();
	_motor_right.stop();
}

}	// namespace leka
