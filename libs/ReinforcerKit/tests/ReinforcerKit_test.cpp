// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ReinforcerKit.h"

#include "LedKitAnimations.h"
#include "MotionKit.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreMotor.h"
#include "mocks/leka/IMUKit.hpp"
#include "mocks/leka/LedKit.h"
#include "mocks/leka/Timeout.h"
#include "mocks/leka/VideoKit.h"

using namespace leka;

using ::testing::AtLeast;
using ::testing::AtMost;

MATCHER_P(isSameAnimation, expected_animation, "")
{
	bool is_same = typeid(*expected_animation) == typeid(*arg);
	return is_same;
}

class ReinforcerkitTest : public ::testing::Test
{
  protected:
	ReinforcerkitTest() : reinforcerkit(mock_videokit, mock_ledkit, motion) {};

	// void SetUp() override {}
	// void TearDown() override {}

	mock::VideoKit mock_videokit {};

	mock::LedKit mock_ledkit;

	mock::CoreMotor mock_motor_left {};
	mock::CoreMotor mock_motor_right {};

	mock::Timeout mock_timeout {};

	mock::IMUKit mock_imukit {};

	MotionKit motion {mock_motor_left, mock_motor_right, mock_imukit, mock_timeout};

	ReinforcerKit reinforcerkit;

	const EulerAngles angles {0.0F, 0.0F, 0.F};

	void expectedCallsMovingReinforcer(interface::LEDAnimation *animation)
	{
		EXPECT_CALL(mock_videokit, playVideoOnce);
		EXPECT_CALL(mock_motor_left, stop).Times(1);
		EXPECT_CALL(mock_motor_right, stop).Times(1);
		EXPECT_CALL(mock_motor_left, spin).Times(AtLeast(1));
		EXPECT_CALL(mock_motor_right, spin).Times(AtLeast(1));
		EXPECT_CALL(mock_timeout, stop).Times(AtMost(1));
		EXPECT_CALL(mock_imukit, getEulerAngles).Times(1);
		EXPECT_CALL(mock_timeout, onTimeout).Times(AtMost(1));
		EXPECT_CALL(mock_timeout, start).Times(AtMost(1));
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(animation)));
	}

	void expectedCallsMotionlessReinforcer(interface::LEDAnimation *animation)
	{
		EXPECT_CALL(mock_videokit, playVideoOnce);
		EXPECT_CALL(mock_ledkit, start(isSameAnimation(animation)));
	}
};

TEST_F(ReinforcerkitTest, initialization)
{
	ASSERT_NE(&reinforcerkit, nullptr);
}

TEST_F(ReinforcerkitTest, playBlinkGreen)
{
	expectedCallsMovingReinforcer(&led::animation::blink_green);

	reinforcerkit.play(ReinforcerKit::Reinforcer::BlinkGreen);

	mock_imukit.call_angles_ready_callback(angles);
}

TEST_F(ReinforcerkitTest, playSpinBlink)
{
	expectedCallsMovingReinforcer(&led::animation::spin_blink);

	reinforcerkit.play(ReinforcerKit::Reinforcer::SpinBlink);

	mock_imukit.call_angles_ready_callback(angles);
}

TEST_F(ReinforcerkitTest, playFire)
{
	expectedCallsMotionlessReinforcer(&led::animation::fire);
	reinforcerkit.play(ReinforcerKit::Reinforcer::Fire);
}

TEST_F(ReinforcerkitTest, playSprinkles)
{
	expectedCallsMotionlessReinforcer(&led::animation::sprinkles);
	reinforcerkit.play(ReinforcerKit::Reinforcer::Sprinkles);
}

TEST_F(ReinforcerkitTest, playRainbow)
{
	expectedCallsMotionlessReinforcer(&led::animation::rainbow);
	reinforcerkit.play(ReinforcerKit::Reinforcer::Rainbow);
}

TEST_F(ReinforcerkitTest, PlayDefaultReinforcer)
{
	expectedCallsMotionlessReinforcer(&led::animation::rainbow);

	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetBlinkGreenAndPlayDefaultReinforcer)
{
	expectedCallsMovingReinforcer(&led::animation::blink_green);

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::BlinkGreen);
	reinforcerkit.playDefault();

	mock_imukit.call_angles_ready_callback(angles);
}

TEST_F(ReinforcerkitTest, SetSpinBlinkAndPlayDefaultReinforcer)
{
	expectedCallsMovingReinforcer(&led::animation::spin_blink);

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::SpinBlink);
	reinforcerkit.playDefault();

	mock_imukit.call_angles_ready_callback(angles);
}

TEST_F(ReinforcerkitTest, SetFireAndPlayDefaultReinforcer)
{
	expectedCallsMotionlessReinforcer(&led::animation::fire);

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Fire);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetSprinklesAndPlayDefaultReinforcer)
{
	expectedCallsMotionlessReinforcer(&led::animation::sprinkles);

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Sprinkles);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, SetRainbowAndPlayDefaultReinforcer)
{
	expectedCallsMotionlessReinforcer(&led::animation::rainbow);

	reinforcerkit.setDefaultReinforcer(ReinforcerKit::Reinforcer::Rainbow);
	reinforcerkit.playDefault();
}

TEST_F(ReinforcerkitTest, stop)
{
	EXPECT_CALL(mock_ledkit, stop);
	EXPECT_CALL(mock_videokit, stopVideo);
	EXPECT_CALL(mock_motor_left, stop);
	EXPECT_CALL(mock_motor_right, stop);
	EXPECT_CALL(mock_timeout, stop);

	reinforcerkit.stop();
}
