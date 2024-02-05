// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "EventLoopKit.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace leka;
using ::testing::MockFunction;

class EventLoopKitTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	EventLoopKit event_loop {};

	MockFunction<void(void)> mock {};
};

TEST_F(EventLoopKitTest, instantiation)
{
	ASSERT_NE(&event_loop, nullptr);
}

TEST_F(EventLoopKitTest, registerCallbackAndStart)
{
	auto loop = [&] {
		mock.Call();
		event_loop.exit();
	};

	// ? As we are using threads, this cannot be tested yet
	EXPECT_CALL(mock, Call()).Times(0);

	event_loop.registerCallback(loop);
	event_loop.start();
}

TEST_F(EventLoopKitTest, registerCallbackAndStartMultipleLoops)
{
	auto loop = [&] {
		mock.Call();
		event_loop.exit();
	};

	// ? As we are using threads, this cannot be tested yet
	EXPECT_CALL(mock, Call()).Times(0);

	event_loop.registerCallback(loop);
	event_loop.start(false);
}

TEST_F(EventLoopKitTest, stop)
{
	auto loop = [&] {
		mock.Call();
		event_loop.exit();
	};

	// ? As we are using threads, this cannot be tested yet
	// ? EXPECT_CALL(mock, Call()).Times(1); uncomment in the future

	event_loop.registerCallback(loop);
	event_loop.start();

	// ? Nothing to test here
	event_loop.stop();
}

TEST_F(EventLoopKitTest, exit)
{
	auto loop = [&] {
		mock.Call();
		event_loop.exit();
	};

	// ? As we are using threads, this cannot be tested yet
	// ? EXPECT_CALL(mock, Call()).Times(1); uncomment in the future

	event_loop.registerCallback(loop);
	event_loop.start();

	// ? Nothing to test here
	event_loop.exit();
}

TEST_F(EventLoopKitTest, startEmptyFunction)
{
	// ? As we are using threads, this cannot be tested yet
	event_loop.registerCallback({});
	event_loop.start();
	// ? Nothing happens
}

TEST_F(EventLoopKitTest, startNullptr)
{
	// ? As we are using threads, this cannot be tested yet
	event_loop.registerCallback(nullptr);
	event_loop.start();
	// ? Nothing happens
}
