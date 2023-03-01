// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "StabilizationControl.hpp"

#include "gtest/gtest.h"

using namespace leka;

class StabilizationControlTest : public ::testing::Test
{
  protected:
	StabilizationControlTest() = default;

	// void SetUp() override {}
	//  void TearDown() override {}

	StabilizationControl stabilization_control {};

	EulerAngles target {.pitch = 0.F, .roll = 0.F, .yaw = 0.F};
	EulerAngles current {};

	float quarterturn_error_speed = 1.F;	 //? mapSpeed(90 * (Kp + Kd)/ KDeltaT)
	float halfturn_error_speed	  = 1.85F;	 //? mapSpeed(180 * (Kp + Kd)/ KDeltaT)
};

TEST_F(StabilizationControlTest, initialization)
{
	ASSERT_NE(&stabilization_control, nullptr);
}

TEST_F(StabilizationControlTest, processStabilizationAngleDefaultPosition)
{
	current = {.pitch = 0.F, .roll = 0.F, .yaw = 0.F};

	auto [speed, direction] = stabilization_control.processStabilizationAngle(target, current);

	EXPECT_EQ(speed, 0.F);
	EXPECT_EQ(direction, Rotation::clockwise);
}

TEST_F(StabilizationControlTest, processStabilizationAngleRolledOverAHalfRight)
{
	current = {.pitch = 0.F, .roll = 0.F, .yaw = 180.F};

	auto [speed, direction] = stabilization_control.processStabilizationAngle(target, current);

	EXPECT_EQ(speed, halfturn_error_speed);
	EXPECT_EQ(direction, Rotation::counterClockwise);
}

TEST_F(StabilizationControlTest, processStabilizationAngleRolledOverAQuarterRight)
{
	current = {.pitch = 0.F, .roll = 0.F, .yaw = 90.F};

	auto [speed, direction] = stabilization_control.processStabilizationAngle(target, current);

	EXPECT_EQ(speed, quarterturn_error_speed);
	EXPECT_EQ(direction, Rotation::counterClockwise);
}

TEST_F(StabilizationControlTest, processStabilizationAngleRolledOverAQuarterLeft)
{
	current = {.pitch = 0.F, .roll = 0.F, .yaw = -90.F};

	auto [speed, direction] = stabilization_control.processStabilizationAngle(target, current);

	EXPECT_EQ(speed, quarterturn_error_speed);
	EXPECT_EQ(direction, Rotation::clockwise);
}

TEST_F(StabilizationControlTest, processStabilizationAngleRolledOverAHalfLeft)
{
	current = {.pitch = 0.F, .roll = 0.F, .yaw = -180.F};

	auto [speed, direction] = stabilization_control.processStabilizationAngle(target, current);

	EXPECT_EQ(speed, halfturn_error_speed);
	EXPECT_EQ(direction, Rotation::clockwise);
}
