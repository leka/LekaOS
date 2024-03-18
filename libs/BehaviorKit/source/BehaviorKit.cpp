// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"
#include <fs_structure.hpp>

#include "rtos/ThisThread.h"

#include "LedKitAnimations.h"

namespace leka {

using namespace std::chrono;

void BehaviorKit::launching()
{
	_videokit.displayImage(fs::home::img::system::robot_misc_splash_screen_large_400);
}

void BehaviorKit::sleeping()
{
	_ledkit.start(&led::animation::sleeping);
	_videokit.playVideoOnce(fs::home::vid::system::robot_system_sleep_yawn_then_sleep_no_eyebrows);
}

void BehaviorKit::waiting()
{
	_ledkit.stop();
	_videokit.playVideoOnRepeat(fs::home::vid::system::robot_system_idle_looking_top_right_left_no_eyebrows);
}

void BehaviorKit::blinkOnCharge()
{
	_ledkit.start(&led::animation::blink_on_charge);
}

void BehaviorKit::lowBattery()
{
	_ledkit.stop();
	_videokit.displayImage(fs::home::img::system::robot_battery_empty_must_be_charged);
	_motor_left.stop();
	_motor_right.stop();
}

void BehaviorKit::mediumLowBattery()
{
	_videokit.playVideoOnce(fs::home::vid::actions::robot_animation_action_yawning_no_eyebrows);
}

void BehaviorKit::chargingEmpty()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_empty_red);
}

void BehaviorKit::chargingLow()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_quarter_1_red);
}

void BehaviorKit::chargingMedium()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_quarter_2_orange);
}

void BehaviorKit::chargingHigh()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_quarter_3_green);
}

void BehaviorKit::chargingFull()
{
	_videokit.displayImage(fs::home::img::system::robot_battery_charging_quarter_4_green);
}

void BehaviorKit::bleConnectionWithoutVideo()
{
	_ledkit.start(&led::animation::ble_connection);
}

void BehaviorKit::bleConnectionWithVideo()
{
	_ledkit.start(&led::animation::ble_connection);
	_videokit.playVideoOnce(fs::home::vid::system::robot_system_ble_connection_wink_no_eyebrows);
}

void BehaviorKit::working()
{
	_videokit.displayImage(fs::home::img::system::robot_face_smiling_slightly);
}

void BehaviorKit::magicCardDetected()
{
	// do nothing
}

void BehaviorKit::fileExchange()
{
	// TODO(@ladislas): add file exchange image
	_videokit.displayImage("/fs/home/img/system/robot-file_exchange.jpg");
}

void BehaviorKit::stop()
{
	_ledkit.stop();
	_videokit.stopVideo();
	_motor_left.stop();
	_motor_right.stop();
}

}	// namespace leka
