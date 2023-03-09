// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MotionKit.hpp"

#include "IMUKit.hpp"
#include "gtest/gtest.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/IMUKit.hpp"
#include "mocks/leka/Timeout.h"

using namespace leka;

using ::testing::_;
using ::testing::AtLeast;

// TODO(@leka/dev-embedded): temporary fix, changes are needed when updating fusion algorithm

ACTION_TEMPLATE(GetCallback, HAS_1_TEMPLATE_PARAMS(typename, callback_t), AND_1_VALUE_PARAMS(pointer))
{
	*pointer = callback_t(arg0);
}

class MotionKitTest : public ::testing::Test
{
  protected:
	MotionKitTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	mock::Timeout mock_timeout {};

	const EulerAngles angles {0.F, 0.F, 0.F};
	const float kOneTurnDegrees = 360.0;

	mock::IMUKit mock_imukit {};

	MotionKit motion {mock_motor_left, mock_motor_right, mock_imukit, mock_timeout};
};

TEST_F(MotionKitTest, initialization)
{
	ASSERT_NE(&motion, nullptr);
}

TEST_F(MotionKitTest, rotateClockwise)
{
	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	EXPECT_CALL(mock_imukit, getEulerAngles).Times(1);

	EXPECT_CALL(mock_timeout, onTimeout).Times(1);
	EXPECT_CALL(mock_timeout, start).Times(1);

	EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, _)).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, _)).Times(AtLeast(1));

	motion.startYawRotation(kOneTurnDegrees, Rotation::clockwise);

	mock_imukit.call_angles_ready_callback(angles);
}

TEST_F(MotionKitTest, rotateCounterClockwise)
{
	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	EXPECT_CALL(mock_imukit, getEulerAngles).Times(1);

	EXPECT_CALL(mock_timeout, onTimeout).Times(1);
	EXPECT_CALL(mock_timeout, start).Times(1);

	EXPECT_CALL(mock_motor_left, spin(Rotation::counterClockwise, _)).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, spin(Rotation::counterClockwise, _)).Times(AtLeast(1));

	motion.startYawRotation(kOneTurnDegrees, Rotation::counterClockwise);

	mock_imukit.call_angles_ready_callback(angles);
}

TEST_F(MotionKitTest, rotateAndStop)
{
	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	EXPECT_CALL(mock_imukit, getEulerAngles).Times(1);

	EXPECT_CALL(mock_timeout, onTimeout).Times(1);
	EXPECT_CALL(mock_timeout, start).Times(1);

	EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, _)).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, _)).Times(AtLeast(1));

	motion.startYawRotation(kOneTurnDegrees, Rotation::clockwise);
	mock_imukit.call_angles_ready_callback(angles);

	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	motion.stop();
}

TEST_F(MotionKitTest, rotateAndTimeOutOver)
{
	interface::Timeout::callback_t on_timeout_callback = {};

	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	EXPECT_CALL(mock_imukit, getEulerAngles).Times(1);

	EXPECT_CALL(mock_timeout, onTimeout).WillOnce(GetCallback<interface::Timeout::callback_t>(&on_timeout_callback));
	EXPECT_CALL(mock_timeout, start).Times(1);

	EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, _)).Times(AtLeast(1));
	EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, _)).Times(AtLeast(1));

	motion.startYawRotation(kOneTurnDegrees, Rotation::clockwise);
	mock_imukit.call_angles_ready_callback(angles);

	EXPECT_CALL(mock_timeout, stop).Times(1);
	EXPECT_CALL(mock_motor_left, stop).Times(1);
	EXPECT_CALL(mock_motor_right, stop).Times(1);

	on_timeout_callback();
}
