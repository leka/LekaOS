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

	EulerAngles angle {.yaw = 0.F};

	const uint8_t kDegreeBoundNullSpeedArea = 3;

	float current_speed;
	float previous_speed = 1000;
};

TEST_F(RotationControlTest, initialization)
{
	ASSERT_NE(&rotation_control, nullptr);
}

TEST_F(RotationControlTest, processRotationAngleCounterClockwise)
{
	rotation_control.setTarget(angle, 1.0);

	for (auto i = 0; i < 180; i += 5) {
		auto current_angle = EulerAngles {.yaw = static_cast<float>(i)};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_LE(current_speed, previous_speed);

		previous_speed = current_speed;
	}

	for (auto i = -180; i < -kDegreeBoundNullSpeedArea; i += 5) {
		auto current_angle = EulerAngles {.yaw = static_cast<float>(i)};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_LE(current_speed, previous_speed);

		previous_speed = current_speed;
	}

	for (auto i = -kDegreeBoundNullSpeedArea; i < 0; i += 1) {
		auto current_angle = EulerAngles {.yaw = static_cast<float>(i)};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_EQ(current_speed, 0.F);
	}
}

TEST_F(RotationControlTest, processRotationAngleClockwise)
{
	rotation_control.setTarget(angle, 1.0);

	for (auto i = 0.F; i > -180; i -= 5) {
		auto current_angle = EulerAngles {.yaw = static_cast<float>(i)};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_LE(current_speed, previous_speed);

		previous_speed = current_speed;
	}

	for (auto i = 180; i > kDegreeBoundNullSpeedArea; i -= 5) {
		auto current_angle = EulerAngles {.yaw = static_cast<float>(i)};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_LE(current_speed, previous_speed);

		previous_speed = current_speed;
	}

	for (auto i = kDegreeBoundNullSpeedArea; i > 0; i -= 1) {
		auto current_angle = EulerAngles {.yaw = static_cast<float>(i)};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_EQ(current_speed, 0.F);
	}
}
