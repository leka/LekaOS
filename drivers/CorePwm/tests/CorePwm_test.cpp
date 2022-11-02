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
	ASSERT_FALSE(spy_PwmOut_is_suspended);
}

TEST(CorePwmTest, write)
{
	auto corepwm = CorePwm {NC};

	corepwm.write(0.5f);

	ASSERT_EQ(spy_PwmOut_value, 0.5f);

	corepwm.write(1.0f);

	ASSERT_EQ(spy_PwmOut_value, 1.0f);
}

TEST(CorePwmTest, read)
{
	auto corepwm = CorePwm {NC};

	corepwm.write(0.5f);

	ASSERT_EQ(spy_PwmOut_value, 0.5f);

	auto val = corepwm.read();

	ASSERT_EQ(val, 0.5f);
}

TEST(CorePwmTest, suspend)
{
	auto corepwm = CorePwm {NC};

	corepwm.suspend();

	ASSERT_TRUE(spy_PwmOut_is_suspended);
}

TEST(CorePwmTest, suspendThenResume)
{
	auto corepwm = CorePwm {NC};

	corepwm.suspend();

	ASSERT_TRUE(spy_PwmOut_is_suspended);

	corepwm.resume();

	ASSERT_FALSE(spy_PwmOut_is_suspended);
}
