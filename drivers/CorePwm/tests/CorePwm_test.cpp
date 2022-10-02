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
