// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ReinforcerKit.h"

#include "LedKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreLED.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/leka/VideoKit.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

using ::testing::InSequence;

class ReinforcerkitTest : public ::testing::Test
{
  protected:
	ReinforcerkitTest() : reinforcerkit(mock_videokit, ledkit, mock_motor_left, mock_motor_right) {};

	// void SetUp() override {}
	// void TearDown() override {}

	mock::VideoKit mock_videokit {};

	mock::CoreLED mock_ears;
	mock::CoreLED mock_belt;

	stub::EventLoopKit stub_event_loop;

	LedKit ledkit {stub_event_loop, mock_ears, mock_belt};

	mock::LEDAnimation mock_animation {};

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	ReinforcerKit reinforcerkit;
};

TEST_F(ReinforcerkitTest, initialization)
{
	ASSERT_NE(&reinforcerkit, nullptr);
}

TEST_F(ReinforcerkitTest, runBlinkGreen)
{
	auto expected_speed = 1;

	EXPECT_CALL(mock_videokit, playVideoOnce);
	{
		InSequence seq;

		EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, expected_speed));
		EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, expected_speed));

		EXPECT_CALL(mock_motor_left, stop());
		EXPECT_CALL(mock_motor_right, stop());
	}

	reinforcerkit.run(Reinforcer::BlinkGreen);
}

TEST_F(ReinforcerkitTest, runSpinBlink)
{
	auto expected_speed = 1;

	EXPECT_CALL(mock_videokit, playVideoOnce);

	{
		InSequence seq;

		EXPECT_CALL(mock_motor_left, spin(Rotation::counterClockwise, expected_speed));
		EXPECT_CALL(mock_motor_right, spin(Rotation::counterClockwise, expected_speed));

		EXPECT_CALL(mock_motor_left, stop());
		EXPECT_CALL(mock_motor_right, stop());
	}

	reinforcerkit.run(Reinforcer::SpinBlink);
}

TEST_F(ReinforcerkitTest, runFire)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	reinforcerkit.run(Reinforcer::Fire);
}

TEST_F(ReinforcerkitTest, runSprinkles)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	reinforcerkit.run(Reinforcer::Sprinkles);
}

TEST_F(ReinforcerkitTest, runRainbow)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	reinforcerkit.run(Reinforcer::Rainbow);
}

TEST_F(ReinforcerkitTest, RunDefaultReinforcer)
{
	auto expected_speed = 1;

	EXPECT_CALL(mock_videokit, playVideoOnce);
	{
		InSequence seq;

		EXPECT_CALL(mock_motor_left, spin(Rotation::clockwise, expected_speed));
		EXPECT_CALL(mock_motor_right, spin(Rotation::clockwise, expected_speed));

		EXPECT_CALL(mock_motor_left, stop());
		EXPECT_CALL(mock_motor_right, stop());
	}

	reinforcerkit.runDefault();
}

TEST_F(ReinforcerkitTest, SetSpinBlinkAndRunDefaultReinforcer)
{
	auto expected_speed = 1;

	EXPECT_CALL(mock_videokit, playVideoOnce);
	{
		InSequence seq;

		EXPECT_CALL(mock_motor_left, spin(Rotation::counterClockwise, expected_speed));
		EXPECT_CALL(mock_motor_right, spin(Rotation::counterClockwise, expected_speed));

		EXPECT_CALL(mock_motor_left, stop());
		EXPECT_CALL(mock_motor_right, stop());
	}

	reinforcerkit.setDefaultReinforcer(Reinforcer::SpinBlink);
	reinforcerkit.runDefault();
}

TEST_F(ReinforcerkitTest, SetFireAndRunDefaultReinforcer)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);

	reinforcerkit.setDefaultReinforcer(Reinforcer::Fire);
	reinforcerkit.runDefault();
}

TEST_F(ReinforcerkitTest, SetSprinklesAndRunDefaultReinforcer)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);

	reinforcerkit.setDefaultReinforcer(Reinforcer::Sprinkles);
	reinforcerkit.runDefault();
}

TEST_F(ReinforcerkitTest, SetRainbowAndRunDefaultReinforcer)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);

	reinforcerkit.setDefaultReinforcer(Reinforcer::Rainbow);
	reinforcerkit.runDefault();
}
