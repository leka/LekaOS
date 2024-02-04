// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <fs_structure.hpp>

#include "rtos/ThisThread.h"

#include "BehaviorKitDeprecated.h"
#include "LedKitAnimations.h"

namespace leka {

using namespace std::chrono;

void BehaviorKitDeprecated::spinLeft(float speed)
{
	_motor_left.spin(Rotation::clockwise, speed);
	_motor_right.spin(Rotation::clockwise, speed);
}

void BehaviorKitDeprecated::spinRight(float speed)
{
	_motor_left.spin(Rotation::counterClockwise, speed);
	_motor_right.spin(Rotation::counterClockwise, speed);
}

void BehaviorKitDeprecated::launching()
{
	_videokit.displayImage(fs::home::img::system::robot_misc_splash_screen_large_400);
}

void BehaviorKitDeprecated::sleeping()
{
	_ledkit.start(&led::animation::sleeping);
	_videokit.playVideoOnce(fs::home::vid::system::robot_system_sleep_yawn_then_sleep_no_eyebrows);
}

void BehaviorKitDeprecated::waiting()
{
	_ledkit.stop();
	_videokit.playVideoOnRepeat(fs::home::vid::system::robot_system_idle_looking_top_right_left_no_eyebrows);
}

void BehaviorKitDeprecated::blinkOnCharge()
{
	_ledkit.start(&led::animation::blink_on_charge);
}

void BehaviorKitDeprecated::lowBattery()
{
	_ledkit.stop();
	_videokit.displayImage(fs::home::img::system::robot_battery_empty_must_be_charged);
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKitDeprecated::chargingEmpty()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_empty_red);
}

void BehaviorKitDeprecated::chargingLow()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_quarter_1_red);
}

void BehaviorKitDeprecated::chargingMedium()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_quarter_2_orange);
}

void BehaviorKitDeprecated::chargingHigh()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_quarter_3_green);
}

void BehaviorKitDeprecated::chargingFull()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_quarter_4_green);
}

void BehaviorKitDeprecated::bleConnectionWithoutVideo()
{
	_ledkit.start(&led::animation::ble_connection);
}

void BehaviorKitDeprecated::bleConnectionWithVideo()
{
	_ledkit.start(&led::animation::ble_connection);
	_videokit.playVideoOnce(fs::home::vid::system::robot_system_ble_connection_wink_no_eyebrows);
}

void BehaviorKitDeprecated::working()
{
	_videokit.displayImage(fs::home::img::system::robot_face_smiling_slightly);
}

void BehaviorKitDeprecated::fileExchange()
{
	// TODO(@ladislas): add file exchange image
	_videokit.displayImage("/fs/home/img/system/robot-file_exchange.jpg");
}

void BehaviorKitDeprecated::stop()
{
	_ledkit.stop();
	_videokit.stopVideo();
	_motor_left.stop();
	_motor_right.stop();
}

}	// namespace leka
