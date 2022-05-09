// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"
#include "rtos/tests/UNITTESTS/doubles/Thread_stub.h"

#include "CoreLED.h"
#include "CoreSPI.h"
#include "LedKit.h"
#include "gtest/gtest.h"
#include "mocks/leka/LEDAnimation.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;
using namespace std::chrono;

using ::testing::AnyNumber;
using ::testing::InSequence;

class LedKitTestAnimations : public ::testing::Test
{
  protected:
	LedKitTestAnimations() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	stub::EventLoopKit event_loop_stub {};

	std::array<RGB, LedKit::kNumberOfLedsBelt> expected_colors {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<LedKit::kNumberOfLedsBelt> belt {spi};
	CoreLED<LedKit::kNumberOfLedsEars> ears {spi};

	LedKit ledkit {event_loop_stub, ears, belt};

	mock::LEDAnimation mock_animation {};
};

TEST_F(LedKitTestAnimations, initialization)
{
	EXPECT_NE(&mock_animation, nullptr);
}

TEST_F(LedKitTestAnimations, init)
{
	ledkit.init();

	std::fill(expected_colors.begin(), expected_colors.end(), RGB::black);

	EXPECT_TRUE(std::equal(expected_colors.begin(), expected_colors.end(), belt.getColor().begin()));
}

TEST_F(LedKitTestAnimations, startAnimation)
{
	event_loop_stub.spy_setNumberOfLoops(10);

	EXPECT_CALL(mock_animation, setLeds).Times(1);
	EXPECT_CALL(mock_animation, start).Times(1);

	ledkit.start(&mock_animation);
}

TEST_F(LedKitTestAnimations, startNullPtr)
{
	EXPECT_CALL(mock_animation, setLeds).Times(0);
	EXPECT_CALL(mock_animation, start).Times(0);

	ledkit.start(nullptr);
}

TEST_F(LedKitTestAnimations, stopWithoutAnimation)
{
	EXPECT_CALL(mock_animation, stop).Times(0);

	ledkit.stop();
}

TEST_F(LedKitTestAnimations, stopStartedAnimation)
{
	EXPECT_CALL(mock_animation, setLeds).Times(1);
	EXPECT_CALL(mock_animation, start).Times(1);
	EXPECT_CALL(mock_animation, stop).Times(1);

	ledkit.start(&mock_animation);
	ledkit.stop();
}

TEST_F(LedKitTestAnimations, startNewAnimationSequence)
{
	mock::LEDAnimation mock_new_animation;

	{
		InSequence seq;

		EXPECT_CALL(mock_animation, setLeds).Times(1);
		EXPECT_CALL(mock_animation, start).Times(1);
		EXPECT_CALL(mock_animation, stop).Times(1);
		EXPECT_CALL(mock_new_animation, setLeds).Times(1);
		EXPECT_CALL(mock_new_animation, start).Times(1);
	}

	ledkit.start(&mock_animation);

	ledkit.start(&mock_new_animation);
}
