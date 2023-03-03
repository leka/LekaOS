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

	void SetUp() override { rotation_control.init(angle, 1.0); }
	//  void TearDown() override {}

	RotationControl rotation_control {};

	EulerAngles angle {.yaw = 0.F};

	float current_speed;
	float previous_speed = 1000;
};

TEST_F(RotationControlTest, initialization)
{
	ASSERT_NE(&rotation_control, nullptr);
}

TEST_F(RotationControlTest, processRotationAngleCounterClockwise)
{
	for (auto i = 0.F; i < 180.F; i += 5.F) {
		auto current_angle = EulerAngles {.yaw = i};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_LE(current_speed, previous_speed);

		previous_speed = current_speed;
	}

	for (auto i = -180.F; i < -5.F; i += 5.F) {
		auto current_angle = EulerAngles {.yaw = i};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_LE(current_speed, previous_speed);

		previous_speed = current_speed;
	}

	for (auto i = -3.F; i < 0.F; i += 1.F) {
		auto current_angle = EulerAngles {.yaw = i};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_EQ(current_speed, 0.F);
	}
}

TEST_F(RotationControlTest, processRotationAngleClockwise)
{
	for (auto i = 0.F; i > -180.F; i -= 5.F) {
		auto current_angle = EulerAngles {.yaw = i};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_LE(current_speed, previous_speed);

		previous_speed = current_speed;
	}

	for (auto i = 180.F; i > 5.F; i -= 5.F) {
		auto current_angle = EulerAngles {.yaw = i};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_LE(current_speed, previous_speed);

		previous_speed = current_speed;
	}

	for (auto i = 3.F; i >= 0.F; i -= 1.F) {
		auto current_angle = EulerAngles {.yaw = i};

		current_speed = rotation_control.processRotationAngle(current_angle);

		EXPECT_EQ(current_speed, 0.F);
	}
}
