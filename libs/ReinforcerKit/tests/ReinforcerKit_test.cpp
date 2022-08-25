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

TEST_F(ReinforcerkitTest, playBlinkGreen)
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

	reinforcerkit.play(ReinforcerKit::Reinforcer::BlinkGreen);
}

TEST_F(ReinforcerkitTest, playSpinBlink)
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

	reinforcerkit.play(ReinforcerKit::Reinforcer::SpinBlink);
}

TEST_F(ReinforcerkitTest, playFire)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	reinforcerkit.play(ReinforcerKit::Reinforcer::Fire);
}

TEST_F(ReinforcerkitTest, playSprinkles)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	reinforcerkit.play(ReinforcerKit::Reinforcer::Sprinkles);
}

TEST_F(ReinforcerkitTest, playRainbow)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);
	reinforcerkit.play(ReinforcerKit::Reinforcer::Rainbow);
}

TEST_F(ReinforcerkitTest, PlayDefaultReinforcer)
{
	auto expected_speed = 1;

	EXPECT_CALL(mock_videokit, playVideoOnce);

	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetSpinBlinkAndPlayDefaultReinforcer)
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

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::SpinBlink);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetFireAndPlayDefaultReinforcer)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Fire);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetSprinklesAndPlayDefaultReinforcer)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Sprinkles);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetRainbowAndPlayDefaultReinforcer)
{
	EXPECT_CALL(mock_videokit, playVideoOnce);

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Rainbow);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, stop)
{
	EXPECT_CALL(mock_videokit, stopVideo);
	EXPECT_CALL(mock_motor_left, stop);
	EXPECT_CALL(mock_motor_right, stop);

	reinforcerkit.stop();
}
