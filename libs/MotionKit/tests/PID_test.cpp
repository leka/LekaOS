// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PID.h"

#include "gtest/gtest.h"

using namespace leka;

class PIDTest : public ::testing::Test
{
  protected:
	PIDTest() = default;

	// void SetUp() override { }
	// void TearDown() override {}

	PID pid {};

	float max_speed_value = 1.8F;	//? ((360-180)*Kp + (360-180)*Kd)/KDeltaT
};

TEST_F(PIDTest, initialization)
{
	ASSERT_NE(&pid, nullptr);
}

TEST_F(PIDTest, processPIDDefaultPosition)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = 180.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, 0.F);
	EXPECT_EQ(direction, Rotation::clockwise);
}

TEST_F(PIDTest, processPIDRolledOverAHalfRight)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = 0.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, max_speed_value);
	EXPECT_EQ(direction, Rotation::clockwise);
}

TEST_F(PIDTest, processPIDRolledOverAQuarterRight)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = 90.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, 0.9F);
	EXPECT_EQ(direction, Rotation::clockwise);
}

TEST_F(PIDTest, processPIDRolledOverAQuarterLeft)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = 270.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, 0.9F);
	EXPECT_EQ(direction, Rotation::counterClockwise);
}

TEST_F(PIDTest, processPIDRolledOverAHalfLeft)
{
	auto pitch = 0.F;
	auto roll  = 0.F;
	auto yaw   = 360.F;

	auto [speed, direction] = pid.processPID(pitch, roll, yaw);

	EXPECT_EQ(speed, max_speed_value);
	EXPECT_EQ(direction, Rotation::counterClockwise);
}
