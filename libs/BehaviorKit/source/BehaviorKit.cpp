// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "rtos/ThisThread.h"

#include "LedKitAnimations.h"

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
	_videokit.displayImage("system/robot-misc-splash_screen-large-400.jpg");
}

void BehaviorKit::sleeping()
{
	_ledkit.start(&led::animation::sleeping);
	_videokit.playVideoOnce("system/robot-system-sleep-yawn_then_sleep-no_eyebrows.avi");
}

void BehaviorKit::waiting()
{
	_ledkit.stop();
	_videokit.playVideoOnRepeat("system/robot-system-idle-looking_top_right_left-no_eyebrows.avi");
}

void BehaviorKit::blinkOnCharge()
{
	_ledkit.start(&led::animation::blink_on_charge);
}

void BehaviorKit::lowBattery()
{
	_ledkit.stop();
	_videokit.displayImage("system/robot-battery-empty-must_be_charged.jpg");
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::chargingEmpty()
{
	_videokit.displayImage("system/robot-battery-charging-empty_red.jpg");
}

void BehaviorKit::chargingLow()
{
	_videokit.displayImage("system/robot-battery-charging-quarter_1-red.jpg");
}

void BehaviorKit::chargingMedium()
{
	_videokit.displayImage("system/robot-battery-charging-quarter_2-orange.jpg");
}

void BehaviorKit::chargingHigh()
{
	_videokit.displayImage("system/robot-battery-charging-quarter_3-green.jpg");
}

void BehaviorKit::chargingFull()
{
	_videokit.displayImage("system/robot-battery-charging-quarter_4-green.jpg");
}

void BehaviorKit::bleConnectionWithoutVideo()
{
	_ledkit.start(&led::animation::ble_connection);
}

void BehaviorKit::bleConnectionWithVideo()
{
	_ledkit.start(&led::animation::ble_connection);
	_videokit.playVideoOnce("system/robot-system-ble_connection-wink-no_eyebrows.avi");
}

void BehaviorKit::working()
{
	_videokit.displayImage("system/robot-face-smiling-slightly.jpg");
}

void BehaviorKit::fileExchange()
{
	_videokit.displayImage("system/robot-file_exchange.jpg");
}

void BehaviorKit::stop()
{
	_ledkit.stop();
	_videokit.stopVideo();
	_motor_left.stop();
	_motor_right.stop();
}

}	// namespace leka
