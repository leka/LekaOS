// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RotationControl.hpp"

#include "gtest/gtest.h"

using namespace leka;

class RotationControlTest : public ::testing::Test
{
  protected:
	RotationControlTest() = default;

	// void SetUp() override {}
	//  void TearDown() override {}

	RotationControl rotation_control {};

	float yaw = 0.F;

	float halfturn_error_speed = 1.85F;			//? mapSpeed(180 * (Kp + Kd)/KDeltaT)
	float one_turn_error_speed = 3.55000019F;	//? mapSpeed(360 * (Kp + Kd)/KDeltaT)
	float two_turn_error_speed = 6.95000029F;	//? mapSpeed(720 * (Kp + Kd)/KDeltaT)
};

TEST_F(RotationControlTest, initialization)
{
	ASSERT_NE(&rotation_control, nullptr);
}

TEST_F(RotationControlTest, processRotationAngleDefaultPosition)
{
	auto target = 0.F;

	auto speed = rotation_control.processRotationAngle(target, yaw);

	EXPECT_EQ(speed, 0.F);
}

TEST_F(RotationControlTest, processRotationAngleHalfTurn)
{
	auto target = 180.F;

	auto speed = rotation_control.processRotationAngle(target, yaw);

	EXPECT_EQ(speed, halfturn_error_speed);
}

TEST_F(RotationControlTest, processRotationAngleOneTurn)
{
	auto target = 360.F;

	auto speed = rotation_control.processRotationAngle(target, yaw);

	EXPECT_EQ(speed, one_turn_error_speed);
}

TEST_F(RotationControlTest, processRotationAngle2Turn)
{
	auto target = 720.F;

	auto speed = rotation_control.processRotationAngle(target, yaw);

	EXPECT_EQ(speed, two_turn_error_speed);
}

TEST_F(RotationControlTest, calculateYawRotationClockwise)
{
	auto previous = -20.F;
	auto yaw	  = 20.F;

	auto abs_diff = rotation_control.calculateYawRotation(previous, yaw);

	EXPECT_EQ(abs_diff, 40.F);
}

TEST_F(RotationControlTest, calculateYawRotationCounterClockwise)
{
	auto previous = 20.F;
	auto yaw	  = -20.F;

	auto abs_diff = rotation_control.calculateYawRotation(previous, yaw);

	EXPECT_EQ(abs_diff, 40.F);
}

TEST_F(RotationControlTest, calculateYawRotationDiscontinuousPoint)
{
	auto previous = -160.F;
	auto yaw	  = 170.F;

	auto abs_diff = rotation_control.calculateYawRotation(previous, yaw);

	EXPECT_EQ(abs_diff, 30.F);
}
