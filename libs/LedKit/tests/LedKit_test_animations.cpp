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
#include "mocks/mbed/EventFlags.h"

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

	rtos::Thread animation_thread;
	mbed::mock::EventFlags mock_event_flags {};

	std::array<RGB, LedKit::kNumberOfLedsBelt> expected_colors {};

	CoreSPI spi {NC, NC, NC, NC};
	CoreLED<LedKit::kNumberOfLedsBelt> belt {spi};
	CoreLED<LedKit::kNumberOfLedsEars> ears {spi};

	LedKit ledkit {animation_thread, mock_event_flags, ears, belt};

	mock::LEDAnimation mock_animation {};
	led::animation::Sleeping sleeping_animation {};

	void MOCK_FUNCTION_silenceUnexpectedCalls() { EXPECT_CALL(mock_animation, start); }
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
	EXPECT_CALL(mock_event_flags, set).Times(2);

	ledkit.start(&mock_animation);
}

TEST_F(LedKitTestAnimations, startNullPtr)
{
	EXPECT_CALL(mock_animation, setLeds).Times(0);
	EXPECT_CALL(mock_event_flags, set).Times(1);

	ledkit.start(nullptr);
}

TEST_F(LedKitTestAnimations, stopWithoutAnimation)
{
	EXPECT_CALL(mock_event_flags, set).Times(1);
	EXPECT_CALL(mock_animation, stop).Times(0);

	ledkit.stop();
}

TEST_F(LedKitTestAnimations, initializeAnimation)
{
	ledkit.start(&mock_animation);

	EXPECT_CALL(mock_animation, setLeds).Times(1);
	EXPECT_CALL(mock_animation, start).Times(1);

	ledkit.initializeAnimation();
}

TEST_F(LedKitTestAnimations, runAnimation)
{
	ledkit.start(&sleeping_animation);
	ledkit.initializeAnimation();

	EXPECT_CALL(mock_event_flags, clear).Times(1);
	EXPECT_CALL(mock_event_flags, get).Times(AnyNumber());

	ledkit.runAnimation();
}

TEST_F(LedKitTestAnimations, stopStartedAnimation)
{
	ledkit.start(&sleeping_animation);

	EXPECT_CALL(mock_event_flags, clear).Times(1);
	EXPECT_CALL(mock_event_flags, get).Times(AnyNumber());
	EXPECT_CALL(mock_event_flags, set).Times(1);

	ledkit.initializeAnimation();
	ledkit.runAnimation();
	ledkit.stop();
}

TEST_F(LedKitTestAnimations, startNewAnimationSequence)
{
	MOCK_FUNCTION_silenceUnexpectedCalls();

	mock::LEDAnimation mock_new_animation;

	{
		InSequence seq;

		EXPECT_CALL(mock_animation, stop);
		EXPECT_CALL(mock_new_animation, start);
	}

	ledkit.start(&mock_animation);
	ledkit.initializeAnimation();

	ledkit.start(&mock_new_animation);
	ledkit.initializeAnimation();
}
