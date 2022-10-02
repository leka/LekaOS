// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;
using ::testing::MockFunction;

class EventLoopKitStubTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	stub::EventLoopKit stub_event_loop {};

	MockFunction<void(void)> mock {};
};

TEST_F(EventLoopKitStubTest, instantiation)
{
	ASSERT_NE(&stub_event_loop, nullptr);
}

TEST_F(EventLoopKitStubTest, registerCallbackAndStart)
{
	auto loop = [&] { mock.Call(); };

	EXPECT_CALL(mock, Call()).Times(1);

	stub_event_loop.registerCallback(loop);
	stub_event_loop.start();
}

TEST_F(EventLoopKitStubTest, spySetNumberOfLoops)
{
	auto loop = [&] { mock.Call(); };

	stub_event_loop.spy_setNumberOfLoops(10);
	EXPECT_CALL(mock, Call()).Times(10);

	stub_event_loop.registerCallback(loop);
	stub_event_loop.start();
}

TEST_F(EventLoopKitStubTest, stop)
{
	auto loop = [&] {
		mock.Call();
		stub_event_loop.stop();
	};

	stub_event_loop.spy_setNumberOfLoops(2);
	EXPECT_CALL(mock, Call()).Times(1);

	stub_event_loop.registerCallback(loop);
	stub_event_loop.start();
}

TEST_F(EventLoopKitStubTest, exit)
{
	auto loop = [&] {
		mock.Call();
		stub_event_loop.exit();
	};

	stub_event_loop.spy_setNumberOfLoops(2);
	EXPECT_CALL(mock, Call()).Times(1);

	stub_event_loop.registerCallback(loop);
	stub_event_loop.start();
}

TEST_F(EventLoopKitStubTest, startEmptyFunction)
{
	stub_event_loop.registerCallback({});
	stub_event_loop.start();
	// ? Nothing happens
}

TEST_F(EventLoopKitStubTest, startNullptr)
{
	stub_event_loop.registerCallback(nullptr);
	stub_event_loop.start();
	// ? Nothing happens
}
