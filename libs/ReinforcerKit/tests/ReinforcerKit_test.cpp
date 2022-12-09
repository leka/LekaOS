// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ReinforcerKit.h"

#include "LedKit.h"
#include "MotionKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/Accelerometer.h"
#include "mocks/leka/CoreLED.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/Gyroscope.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/leka/VideoKit.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

using ::testing::AnyNumber;
using ::testing::Sequence;

class ReinforcerkitTest : public ::testing::Test
{
  protected:
	ReinforcerkitTest() : reinforcerkit(mock_videokit, ledkit, motion) {};

	// void SetUp() override {}
	// void TearDown() override {}

	mock::VideoKit mock_videokit {};

	mock::CoreLED mock_ears;
	mock::CoreLED mock_belt;

	stub::EventLoopKit stub_event_loop;

	LedKit ledkit {stub_event_loop, mock_ears, mock_belt};

	mock::LEDAnimation mock_animation {};

	stub::EventLoopKit stub_event_loop_imu {};
	stub::EventLoopKit stub_event_loop_motion {};

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	mock::Accelerometer accel {};
	mock::Gyroscope gyro {};

	IMUKit imukit {stub_event_loop_imu, accel, gyro};

	MotionKit motion {mock_motor_left, mock_motor_right, imukit, stub_event_loop_motion};

	ReinforcerKit reinforcerkit;

	void expectedCallsMovingReinforcer()
	{
		EXPECT_CALL(mock_videokit, playVideoOnce);
		EXPECT_CALL(mock_motor_left, stop).Times(1);
		EXPECT_CALL(mock_motor_right, stop).Times(1);
		EXPECT_CALL(mock_motor_left, spin).Times(1);
		EXPECT_CALL(mock_motor_right, spin).Times(1);
		EXPECT_CALL(mock_ears, setColor).Times(AnyNumber());
		EXPECT_CALL(mock_ears, show).Times(AnyNumber());
		EXPECT_CALL(mock_belt, setColor).Times(AnyNumber());
		EXPECT_CALL(mock_belt, show).Times(AnyNumber());
	}

	void expectedCallsMotionlessReinforcer()
	{
		EXPECT_CALL(mock_videokit, playVideoOnce);
		EXPECT_CALL(mock_ears, setColor).Times(AnyNumber());
		EXPECT_CALL(mock_ears, show).Times(AnyNumber());
		EXPECT_CALL(mock_belt, setColor).Times(AnyNumber());
		EXPECT_CALL(mock_belt, show).Times(AnyNumber());
	}
};

TEST_F(ReinforcerkitTest, initialization)
{
	ASSERT_NE(&reinforcerkit, nullptr);
}

TEST_F(ReinforcerkitTest, playBlinkGreen)
{
	expectedCallsMovingReinforcer();

	reinforcerkit.play(ReinforcerKit::Reinforcer::BlinkGreen);
}

TEST_F(ReinforcerkitTest, playSpinBlink)
{
	expectedCallsMovingReinforcer();

	reinforcerkit.play(ReinforcerKit::Reinforcer::SpinBlink);
}

TEST_F(ReinforcerkitTest, playFire)
{
	expectedCallsMotionlessReinforcer();
	reinforcerkit.play(ReinforcerKit::Reinforcer::Fire);
}

TEST_F(ReinforcerkitTest, playSprinkles)
{
	expectedCallsMotionlessReinforcer();
	reinforcerkit.play(ReinforcerKit::Reinforcer::Sprinkles);
}

TEST_F(ReinforcerkitTest, playRainbow)
{
	expectedCallsMotionlessReinforcer();
	reinforcerkit.play(ReinforcerKit::Reinforcer::Rainbow);
}

TEST_F(ReinforcerkitTest, PlayDefaultReinforcer)
{
	expectedCallsMotionlessReinforcer();

	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetBlinkGreenAndPlayDefaultReinforcer)
{
	expectedCallsMovingReinforcer();

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::BlinkGreen);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetSpinBlinkAndPlayDefaultReinforcer)
{
	expectedCallsMovingReinforcer();

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::SpinBlink);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetFireAndPlayDefaultReinforcer)
{
	expectedCallsMotionlessReinforcer();

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Fire);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetSprinklesAndPlayDefaultReinforcer)
{
	expectedCallsMotionlessReinforcer();

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Sprinkles);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetRainbowAndPlayDefaultReinforcer)
{
	expectedCallsMotionlessReinforcer();

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
