// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreInterruptIn.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/mbed/InterruptIn.h"

using namespace leka;
using ::testing::MockFunction;

class CoreInterruptInTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	CoreInterruptIn interrupt_in {NC};
};

TEST_F(CoreInterruptInTest, initialisation)
{
	ASSERT_NE(&interrupt_in, nullptr);
}

TEST_F(CoreInterruptInTest, readPinDown)
{
	auto expected_pin_down_value = 0;
	auto pin_value				 = interrupt_in.read();

	EXPECT_EQ(pin_value, expected_pin_down_value);
}

TEST_F(CoreInterruptInTest, readPinUp)
{
	auto expected_pin_up_value = 1;
	spy_InterruptIn_setValue(1);
	auto pin_value = interrupt_in.read();

	EXPECT_EQ(pin_value, expected_pin_up_value);
}

TEST_F(CoreInterruptInTest, rise)
{
	MockFunction<void()> mock_function {};
	EXPECT_CALL(mock_function, Call).Times(1);

	interrupt_in.rise(mock_function.AsStdFunction());

	auto on_rise_callback = spy_InterruptIn_getRiseCallback();
	on_rise_callback();
}

TEST_F(CoreInterruptInTest, fall)
{
	MockFunction<void()> mock_function {};
	EXPECT_CALL(mock_function, Call).Times(1);

	interrupt_in.fall(mock_function.AsStdFunction());

	auto on_fall_callback = spy_InterruptIn_getFallCallback();
	on_fall_callback();
}
