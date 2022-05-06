// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LedKit.h"

#include "rtos/tests/UNITTESTS/doubles/Thread_stub.h"

#include "CoreLED.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreLED.h"
#include "mocks/leka/LEDAnimation.h"
#include "mocks/mbed/EventFlags.h"

using namespace leka;

using ::testing::InSequence;

class LedKitTest : public ::testing::Test
{
  protected:
	LedKitTest() = default;

	// void SetUp() override {}
	// void TearDown() override {}

	std::array<RGB, LedKit::kNumberOfLedsBelt> expected_colors {};

	mock::CoreLED mock_ears;
	mock::CoreLED mock_belt;

	rtos::Thread animation_thread;
	mbed::mock::EventFlags mock_event_flags;

	LedKit ledkit {animation_thread, mock_event_flags, mock_ears, mock_belt};
};

TEST_F(LedKitTest, initialization)
{
	EXPECT_NE(&ledkit, nullptr);
}
