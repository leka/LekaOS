// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <chrono>

#include "rtos/ThisThread.h"

#include "gtest/gtest.h"

using namespace std::chrono;

TEST(UnitTestsSetup, mbedConfigAvailable)
{
	// Test mbed_config.h is available
	ASSERT_STREQ(MBED_CONF_APP_TARGET_NAME, "LEKA_Vx_x_UNIT_TEST");
}

TEST(UnitTestsSetup, mbedTargetConfigAvailable)
{
	// Test mbed_target_config.h is available
	ASSERT_EQ(HSE_VALUE, 25000000);
}

TEST(UnitTestsSetup, mbedStubsAvailable)
{
	// Test mbed stubs are available
	rtos::ThisThread::sleep_for(500ms);
	EXPECT_FALSE(false);
}
