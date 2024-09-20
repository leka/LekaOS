// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BehaviorKitDeprecated.h"

#include "LedKitAnimations.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/LedKit.h"
#include "mocks/leka/VideoKit.h"

using namespace leka;

using ::testing::InSequence;

MATCHER_P(isSameAnimation, expected_animation_type, "")
{
	bool is_same = typeid(*expected_animation_type) == typeid(*arg);
	return is_same;
}

class BehaviorKitDeprecatedTest : public ::testing::Test
{
  protected:
	BehaviorKitDeprecatedTest() : behaviorkit(mock_videokit, mock_ledkit, mock_motor_left, mock_motor_right) {};

	// void SetUp() override {}
	// void TearDown() override {}

	mock::VideoKit mock_videokit {};

	mock::LedKit mock_ledkit {};

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	BehaviorKitDeprecated behaviorkit;
};

TEST_F(BehaviorKitDeprecatedTest, initialization)
{
	ASSERT_NE(&behaviorkit, nullptr);
}

TEST_F(BehaviorKitDeprecatedTest, spinLeftAnySpeed)
{
	auto expected_speed = 0.7;

	EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, expected_speed));
	EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, expected_speed));

	behaviorkit.spinLeft(expected_speed);
}

TEST_F(BehaviorKitDeprecatedTest, spinRightAnySpeed)
{
	auto expected_speed = 0.3;

	EXPECT_CALL(mock_motor_left, spin(Rotation::counterClockwise, expected_speed));
	EXPECT_CALL(mock_motor_right, spin(Rotation::counterClockwise, expected_speed));

	behaviorkit.spinRight(expected_speed);
}

TEST_F(BehaviorKitDeprecatedTest, launching)
{
	EXPECT_CALL(mock_videokit, displayImage);
	behaviorkit.launching();
}

TEST_F(BehaviorKitDeprecatedTest, sleeping)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::sleeping))).Times(1);

	behaviorkit.sleeping();
}

TEST_F(BehaviorKitDeprecatedTest, waiting)
{
	EXPECT_CALL(mock_ledkit, stop);
	EXPECT_CALL(mock_videokit, playVideoOnRepeat);
	behaviorkit.waiting();
}

TEST_F(BehaviorKitDeprecatedTest, batteryBehaviors)
{
	EXPECT_CALL(mock_videokit, displayImage).Times(6);
	EXPECT_CALL(mock_ledkit, stop);
	EXPECT_CALL(mock_motor_left, stop()).Times(1);
	EXPECT_CALL(mock_motor_right, stop()).Times(1);

	behaviorkit.lowBattery();
	behaviorkit.chargingEmpty();
	behaviorkit.chargingLow();
	behaviorkit.chargingMedium();
	behaviorkit.chargingHigh();
	behaviorkit.chargingFull();
}

TEST_F(BehaviorKitDeprecatedTest, bleConnectionWithoutVideo)
{
	EXPECT_CALL(mock_videokit, playVideoOnce).Times(0);
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::ble_connection))).Times(1);

	behaviorkit.bleConnectionWithoutVideo();
}

TEST_F(BehaviorKitDeprecatedTest, bleConnectionWithVideo)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	EXPECT_CALL(mock_ledkit, start(isSameAnimation(&led::animation::ble_connection))).Times(1);

	behaviorkit.bleConnectionWithVideo();
}

TEST_F(BehaviorKitDeprecatedTest, working)
{
	EXPECT_CALL(mock_videokit, displayImage);
	behaviorkit.working();
}

TEST_F(BehaviorKitDeprecatedTest, fileExchange)
{
	EXPECT_CALL(mock_videokit, displayImage);
	behaviorkit.fileExchange();
}

TEST_F(BehaviorKitDeprecatedTest, stop)
{
	EXPECT_CALL(mock_ledkit, stop);
	EXPECT_CALL(mock_videokit, stopVideo);
	EXPECT_CALL(mock_motor_left, stop());
	EXPECT_CALL(mock_motor_right, stop());

	behaviorkit.stop();
}
