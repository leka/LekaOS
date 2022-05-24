// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "rtos/tests/UNITTESTS/doubles/Thread_stub.h"

#include "CorePwm.h"
#include "LedKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreLED.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/leka/PwmOut.h"
#include "mocks/leka/VideoKit.h"
#include "mocks/mbed/DigitalOut.h"
#include "mocks/mbed/EventFlags.h"

using namespace leka;

using ::testing::InSequence;

class BehaviorKitTest : public ::testing::Test
{
  protected:
	BehaviorKitTest() : behaviorkit(mock_videokit, ledkit, mock_motor_left, mock_motor_right) {};

	// void SetUp() override {}
	// void TearDown() override {}

	mock::VideoKit mock_videokit {};

	mock::CoreLED mock_ears;
	mock::CoreLED mock_belt;

	rtos::Thread animation_thread;
	mbed::mock::EventFlags mock_event_flags;

	LedKit ledkit {animation_thread, mock_event_flags, mock_ears, mock_belt};

	mock::LEDAnimation mock_animation {};

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	BehaviorKit behaviorkit;
};

TEST_F(BehaviorKitTest, initialization)
{
	ASSERT_NE(&behaviorkit, nullptr);
}

TEST_F(BehaviorKitTest, spinLeftAnySpeed)
{
	auto expected_speed = 0.7;

	EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, expected_speed));
	EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, expected_speed));

	behaviorkit.spinLeft(expected_speed);
}

TEST_F(BehaviorKitTest, spinRightAnySpeed)
{
	auto expected_speed = 0.3;

	EXPECT_CALL(mock_motor_left, spin(Rotation::counterClockwise, expected_speed));
	EXPECT_CALL(mock_motor_right, spin(Rotation::counterClockwise, expected_speed));

	behaviorkit.spinRight(expected_speed);
}

TEST_F(BehaviorKitTest, launching)
{
	EXPECT_CALL(mock_videokit, displayImage);
	behaviorkit.launching();
}

TEST_F(BehaviorKitTest, sleeping)
{
	EXPECT_CALL(mock_videokit, playVideo);
	behaviorkit.sleeping();
}

TEST_F(BehaviorKitTest, waiting)
{
	EXPECT_CALL(mock_videokit, playVideo);
	behaviorkit.waiting();
}

TEST_F(BehaviorKitTest, batteryBehaviors)
{
	EXPECT_CALL(mock_videokit, displayImage).Times(6);

	behaviorkit.lowBattery();
	behaviorkit.chargingZero();
	behaviorkit.chargingRed();
	behaviorkit.chargingOrange();
	behaviorkit.chargingYellow();
	behaviorkit.chargingGreen();
}

TEST_F(BehaviorKitTest, blinkGreen)
{
	auto expected_speed = 1;

	EXPECT_CALL(mock_videokit, playVideo);
	{
		InSequence seq;

		EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, expected_speed));
		EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, expected_speed));

		EXPECT_CALL(mock_motor_left, stop());
		EXPECT_CALL(mock_motor_right, stop());
	}

	behaviorkit.blinkGreen();
}

TEST_F(BehaviorKitTest, spinBlink)
{
	auto expected_speed = 1;

	EXPECT_CALL(mock_videokit, playVideo);

	{
		InSequence seq;

		EXPECT_CALL(mock_motor_left, spin(Rotation::counterClockwise, expected_speed));
		EXPECT_CALL(mock_motor_right, spin(Rotation::counterClockwise, expected_speed));

		EXPECT_CALL(mock_motor_left, stop());
		EXPECT_CALL(mock_motor_right, stop());
	}

	behaviorkit.spinBlink();
}

TEST_F(BehaviorKitTest, fire)
{
	EXPECT_CALL(mock_videokit, playVideo);
	behaviorkit.fire();
}

TEST_F(BehaviorKitTest, sprinkles)
{
	EXPECT_CALL(mock_videokit, playVideo);
	behaviorkit.sprinkles();
}

TEST_F(BehaviorKitTest, rainbow)
{
	EXPECT_CALL(mock_videokit, playVideo);
	behaviorkit.rainbow();
}

TEST_F(BehaviorKitTest, bleConnectionWhileCharging)
{
	EXPECT_CALL(mock_videokit, playVideo).Times(0);
	behaviorkit.bleConnection(false);
}

TEST_F(BehaviorKitTest, bleConnectionWhileNotCharging)
{
	EXPECT_CALL(mock_videokit, playVideo);
	behaviorkit.bleConnection(true);
}

TEST_F(BehaviorKitTest, stop)
{
	auto speed = 0.5;

	ledkit.start(&mock_animation);

	EXPECT_CALL(mock_videokit, stopVideo);
	EXPECT_CALL(mock_animation, stop);

	behaviorkit.stop();
}
