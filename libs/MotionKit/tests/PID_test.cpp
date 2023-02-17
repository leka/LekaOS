// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PID.hpp"

#include "gtest/gtest.h"

using namespace leka;

class PIDTest : public ::testing::Test
{
  protected:
	PIDTest() = default;

	void SetUp() override { pid.setTargetYaw(kTargetYaw); }
	// void TearDown() override {}

	PID pid {};

	float kTargetYaw = 0;

	float halfturn_error_speed = 6.3F;	 //? ((360-180)*Kp + (360-180)*Kd)/KDeltaT
};

TEST_F(PIDTest, initialization)
{
	ASSERT_NE(&pid, nullptr);
}

TEST_F(PIDTest, processPIDDefaultPosition)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = 0.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, 0.F);
	EXPECT_EQ(direction, Rotation::clockwise);
}

TEST_F(PIDTest, processPIDRolledOverAHalfRight)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = 180.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, halfturn_error_speed);
	EXPECT_EQ(direction, Rotation::counterClockwise);
}

TEST_F(PIDTest, processPIDRolledOverAQuarterRight)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = 90.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, halfturn_error_speed / 2.F);
	EXPECT_EQ(direction, Rotation::counterClockwise);
}

TEST_F(PIDTest, processPIDRolledOverAQuarterLeft)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = -90.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, halfturn_error_speed / 2.F);
	EXPECT_EQ(direction, Rotation::clockwise);
}

TEST_F(PIDTest, processPIDRolledOverAHalfLeft)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = -180.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, halfturn_error_speed);
	EXPECT_EQ(direction, Rotation::clockwise);
}

TEST_F(PIDTest, processPIDByErrorDefaultPosition)
{
	auto error = 0.F;

	auto speed = pid.processPIDByError(error);

	EXPECT_EQ(speed, 0.F);
}

TEST_F(PIDTest, processPIDByErrorHalfTurn)
{
	auto error = 180.F;

	auto speed = pid.processPIDByError(error);

	EXPECT_EQ(speed, halfturn_error_speed);
}

TEST_F(PIDTest, processPIDByErrorOneTurn)
{
	auto error = 360.F;

	auto speed = pid.processPIDByError(error);

	EXPECT_EQ(speed, halfturn_error_speed * 2);
}

TEST_F(PIDTest, processPIDByError2Turn)
{
	auto error = 720.F;

	auto speed = pid.processPIDByError(error);

	EXPECT_EQ(speed, halfturn_error_speed * 4);
}
