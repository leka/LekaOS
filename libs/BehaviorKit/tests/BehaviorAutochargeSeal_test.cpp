// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "behaviors/AutochargeSeal.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/IMUKit.hpp"

using namespace leka;

using ::testing::_;
using ::testing::Return;

class AutochargeSealTest : public ::testing::Test
{
  protected:
	AutochargeSealTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};
	mock::IMUKit mock_imukit {};

	behavior::AutochargeSeal autocharge_seal {mock_motor_left, mock_motor_right, mock_imukit};

	void expectedCallsSpinLeft()
	{
		EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, _));
		EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, _));
	}

	void expectedCallsSpinRight()
	{
		EXPECT_CALL(mock_motor_left, spin(Rotation::counterClockwise, _));
		EXPECT_CALL(mock_motor_right, spin(Rotation::counterClockwise, _));
	}

	void expectedCallsMoveForward()
	{
		EXPECT_CALL(mock_motor_left, spin(Rotation::counterClockwise, _));
		EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, _));
	}

	void expectedCallsMoveBackward()
	{
		EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, _));
		EXPECT_CALL(mock_motor_right, spin(Rotation::counterClockwise, _));
	}
};

TEST_F(AutochargeSealTest, initialization)
{
	ASSERT_NE(&autocharge_seal, nullptr);
}

TEST_F(AutochargeSealTest, id)
{
	ASSERT_EQ(autocharge_seal.id(), behavior_id::autocharge_seal);
}

TEST_F(AutochargeSealTest, runNothingToFix)
{
	auto angles = EulerAngles {0, 0, 0};

	EXPECT_CALL(mock_imukit, getEulerAngles).WillOnce(Return(angles));
	EXPECT_CALL(mock_motor_left, stop);
	EXPECT_CALL(mock_motor_right, stop);

	autocharge_seal.run();
}

TEST_F(AutochargeSealTest, runFixPositivePitch)
{
	auto angles = EulerAngles {10, 0, 0};

	EXPECT_CALL(mock_imukit, getEulerAngles).WillOnce(Return(angles));
	expectedCallsMoveForward();

	autocharge_seal.run();
}

TEST_F(AutochargeSealTest, runFixNegativePitch)
{
	auto angles = EulerAngles {-10, 0, 0};

	EXPECT_CALL(mock_imukit, getEulerAngles).WillOnce(Return(angles));
	expectedCallsMoveBackward();

	autocharge_seal.run();
}

TEST_F(AutochargeSealTest, runFixPositiveRollPositivePitch)
{
	auto angles = EulerAngles {10, 10, 0};

	EXPECT_CALL(mock_imukit, getEulerAngles).WillOnce(Return(angles));
	expectedCallsSpinRight();

	autocharge_seal.run();
}

TEST_F(AutochargeSealTest, runFixPositiveRollNegativePitch)
{
	auto angles = EulerAngles {-10, 10, 0};

	EXPECT_CALL(mock_imukit, getEulerAngles).WillOnce(Return(angles));
	expectedCallsSpinLeft();

	autocharge_seal.run();
}

TEST_F(AutochargeSealTest, runFixNegativeRollPositivePitch)
{
	auto angles = EulerAngles {10, -10, 0};

	EXPECT_CALL(mock_imukit, getEulerAngles).WillOnce(Return(angles));
	expectedCallsSpinLeft();

	autocharge_seal.run();
}

TEST_F(AutochargeSealTest, runFixNegativeRollNegativePitch)
{
	auto angles = EulerAngles {-10, -10, 0};

	EXPECT_CALL(mock_imukit, getEulerAngles).WillOnce(Return(angles));
	expectedCallsSpinRight();

	autocharge_seal.run();
}

TEST_F(AutochargeSealTest, stop)
{
	EXPECT_CALL(mock_motor_left, stop);
	EXPECT_CALL(mock_motor_right, stop);

	autocharge_seal.stop();
}
