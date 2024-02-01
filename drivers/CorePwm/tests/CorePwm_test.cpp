// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CorePwm.h"

#include "gtest/gtest.h"
#include "stubs/mbed/PwmOut.h"

using namespace leka;

TEST(CorePwmTest, initialisation)
{
	auto corepwm = CorePwm {NC};

	ASSERT_NE(&corepwm, nullptr);
	ASSERT_FALSE(spy_PwmOut_isSuspended());
}

TEST(CorePwmTest, write)
{
	auto corepwm = CorePwm {NC};

	corepwm.write(0.5f);

	ASSERT_EQ(spy_PwmOut_getValue(), 0.5f);

	corepwm.write(1.0f);

	ASSERT_EQ(spy_PwmOut_getValue(), 1.0f);
}

TEST(CorePwmTest, read)
{
	auto corepwm = CorePwm {NC};

	corepwm.write(0.5f);

	ASSERT_EQ(spy_PwmOut_getValue(), 0.5f);

	auto val = corepwm.read();

	ASSERT_EQ(val, 0.5f);
}

TEST(CorePwmTest, period)
{
	auto corepwm = CorePwm {NC};
	auto value	 = 1.F / 100.F;

	corepwm.period(value);

	ASSERT_EQ(spy_PwmOut_getPeriod(), value);
}

TEST(CorePwmTest, enableDeepSleep)
{
	auto corepwm = CorePwm {NC};

	corepwm.enableDeepSleep();
	ASSERT_TRUE(spy_PwmOut_isSuspended());
}

TEST(CorePwmTest, disableDeepSleep)
{
	auto corepwm = CorePwm {NC};

	corepwm.disableDeepSleep();
	ASSERT_FALSE(spy_PwmOut_isSuspended());
}

TEST(CorePwmTest, enableDeepSleepThenDisableDeepSleepThenEnableDeepSleep)
{
	auto corepwm = CorePwm {NC};

	corepwm.enableDeepSleep();
	ASSERT_TRUE(spy_PwmOut_isSuspended());

	corepwm.disableDeepSleep();
	ASSERT_FALSE(spy_PwmOut_isSuspended());

	corepwm.enableDeepSleep();
	ASSERT_TRUE(spy_PwmOut_isSuspended());
}
