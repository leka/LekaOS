// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <chrono>

#include "rtos/ThisThread.h"

#include "gtest/gtest.h"

using namespace std::chrono;

TEST(UnitTestsSetup, mbed_config_available)
{
	// Test mbed_config.h is available
	ASSERT_STREQ(MBED_CONF_APP_TARGET_NAME, "LEKA_V1_0_DEV");
}

TEST(UnitTestsSetup, mbed_target_config_available)
{
	// Test mbed_target_config.h is available
	ASSERT_EQ(HSE_VALUE, 25000000);
}

TEST(UnitTestsSetup, mbed_stubs_available)
{
	// Test mbed stubs are available
	rtos::ThisThread::sleep_for(500ms);
	EXPECT_FALSE(false);
}
