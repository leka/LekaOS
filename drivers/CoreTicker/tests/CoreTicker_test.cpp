// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreTicker.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;
using namespace std::chrono;

using ::testing::MockFunction;

TEST(CoreTickerTest, initialisation)
{
	auto coreticker = CoreTicker {};

	EXPECT_NE(&coreticker, nullptr);
}

TEST(CoreTickerTest, attachStartStop)
{
	MockFunction<void(void)> mock {};

	auto func = [&] { mock.Call(); };

	EXPECT_CALL(mock, Call()).Times(1);

	auto coreticker = CoreTicker {};

	coreticker.onTick(func);
	coreticker.start(10s);
	coreticker.stop();
}

TEST(CoreTickerTest, attachDetachStart)
{
	MockFunction<void(void)> mock1 {};
	MockFunction<void(void)> mock2 {};

	auto func1 = [&] { mock1.Call(); };
	auto func2 = [&] { mock2.Call(); };

	EXPECT_CALL(mock1, Call()).Times(0);
	EXPECT_CALL(mock2, Call()).Times(1);

	auto coreticker = CoreTicker {};

	coreticker.onTick(func1);
	coreticker.onTick(func2);
	coreticker.start(10s);
}
