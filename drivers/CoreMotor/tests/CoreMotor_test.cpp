// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMotor.h"

#include "gtest/gtest.h"
#include "mocks/leka/PwmOut.h"
#include "mocks/mbed/DigitalOut.h"

using ::testing::_;
using ::testing::AnyNumber;

using namespace leka;

class CoreMotorTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	mbed::mock::DigitalOut dir_1 = {};
	mbed::mock::DigitalOut dir_2 = {};
	mock::PwmOut speed			 = {};

	// TODO (@ladislas) - These EXPECT_CALL suppress the GMOCK WARNING: Uninteresting mock function call
	// TODO (@ladislas) - Remove them in the future
	void MOCK_FUNCTION_silence_digital_write_unexpected_calls()
	{
		EXPECT_CALL(dir_1, write).Times(AnyNumber());
		EXPECT_CALL(dir_2, write).Times(AnyNumber());
	}
};

TEST_F(CoreMotorTest, initialization)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	ASSERT_NE(&motor, nullptr);
}

TEST_F(CoreMotorTest, suspendOnInitialization)
{
	mbed::mock::DigitalOut local_dir_1 = {};
	mbed::mock::DigitalOut local_dir_2 = {};
	mock::PwmOut local_speed		   = {};

	EXPECT_CALL(local_speed, suspend());

	auto local_motor = CoreMotor {local_dir_1, local_dir_2, local_speed};
}

TEST_F(CoreMotorTest, rotateClockwiseNormalSpeed)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	EXPECT_CALL(dir_1, write(1));
	EXPECT_CALL(dir_2, write(0));
	EXPECT_CALL(speed, write(0.5));
	EXPECT_CALL(speed, resume());

	motor.spin(Rotation::clockwise, 0.5);
}

TEST_F(CoreMotorTest, rotateClockwiseMaxSpeed)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	EXPECT_CALL(dir_1, write(1));
	EXPECT_CALL(dir_2, write(0));
	EXPECT_CALL(speed, write(1));
	EXPECT_CALL(speed, resume());

	motor.spin(Rotation::clockwise, 1);
}

TEST_F(CoreMotorTest, rotateCounterClockwiseNormalSpeed)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	EXPECT_CALL(dir_1, write(0));
	EXPECT_CALL(dir_2, write(1));
	EXPECT_CALL(speed, write(0.5));
	EXPECT_CALL(speed, resume());

	motor.spin(Rotation::counterClockwise, 0.5);
}

TEST_F(CoreMotorTest, rotateCounterClockwiseMaxSpeed)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	EXPECT_CALL(dir_1, write(0));
	EXPECT_CALL(dir_2, write(1));
	EXPECT_CALL(speed, write(1));
	EXPECT_CALL(speed, resume());

	motor.spin(Rotation::counterClockwise, 1);
}

TEST_F(CoreMotorTest, stop)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	EXPECT_CALL(dir_1, write(0));
	EXPECT_CALL(dir_2, write(0));
	EXPECT_CALL(speed, write(0));
	EXPECT_CALL(speed, suspend());

	motor.stop();
}

TEST_F(CoreMotorTest, speedValueEqualToZero)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	MOCK_FUNCTION_silence_digital_write_unexpected_calls();

	EXPECT_CALL(speed, write(0));
	EXPECT_CALL(speed, suspend());

	motor.spin(Rotation::clockwise, 0);
}

TEST_F(CoreMotorTest, speedValueNotGreaterThanOne)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	MOCK_FUNCTION_silence_digital_write_unexpected_calls();

	EXPECT_CALL(speed, write(1));
	EXPECT_CALL(speed, resume());

	motor.spin(Rotation::clockwise, 100);
}

TEST_F(CoreMotorTest, speedValueNotLowerThanZero)
{
	EXPECT_CALL(speed, suspend());
	auto motor = CoreMotor {dir_1, dir_2, speed};

	MOCK_FUNCTION_silence_digital_write_unexpected_calls();

	EXPECT_CALL(speed, write(0));
	EXPECT_CALL(speed, suspend());

	motor.spin(Rotation::clockwise, -100);
}
