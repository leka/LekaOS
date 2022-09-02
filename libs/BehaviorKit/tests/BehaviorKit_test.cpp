// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKit.h"

#include "LedKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreLED.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/leka/VideoKit.h"
#include "stubs/leka/EventLoopKit.h"

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

	stub::EventLoopKit stub_event_loop;

	LedKit ledkit {stub_event_loop, mock_ears, mock_belt};

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
	EXPECT_CALL(mock_videokit, playVideoOnce);
	behaviorkit.sleeping();
}

TEST_F(BehaviorKitTest, waiting)
{
	EXPECT_CALL(mock_videokit, playVideoOnRepeat);
	behaviorkit.waiting();
}

TEST_F(BehaviorKitTest, batteryBehaviors)
{
	EXPECT_CALL(mock_videokit, displayImage).Times(6);
	EXPECT_CALL(mock_motor_left, stop()).Times(1);
	EXPECT_CALL(mock_motor_right, stop()).Times(1);

	behaviorkit.lowBattery();
	behaviorkit.chargingEmpty();
	behaviorkit.chargingLow();
	behaviorkit.chargingMedium();
	behaviorkit.chargingHigh();
	behaviorkit.chargingFull();
}

TEST_F(BehaviorKitTest, bleConnectionWhileCharging)
{
	EXPECT_CALL(mock_videokit, playVideoOnce).Times(0);
	behaviorkit.bleConnection(false);
}

TEST_F(BehaviorKitTest, bleConnectionWhileNotCharging)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	behaviorkit.bleConnection(true);
}

TEST_F(BehaviorKitTest, working)
{
	EXPECT_CALL(mock_videokit, displayImage);
	behaviorkit.working();
}

TEST_F(BehaviorKitTest, displayAutonomousActivitiesPrompt)
{
	EXPECT_CALL(mock_videokit, displayImage);
	behaviorkit.displayAutonomousActivitiesPrompt();
}

TEST_F(BehaviorKitTest, stop)
{
	auto speed = 0.5;

	ledkit.start(&mock_animation);

	EXPECT_CALL(mock_videokit, stopVideo);
	EXPECT_CALL(mock_animation, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop());
	EXPECT_CALL(mock_motor_right, stop());

	behaviorkit.stop();
}
