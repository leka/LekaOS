// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreEventQueue.h"
#include <chrono>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/EventQueue.h"

using namespace leka;
using namespace std::chrono;

using ::testing::MockFunction;

class CoreEventQueueTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	CoreEventQueue event_queue {};

	// ? Instantiation of mock::EventQueue is needed to setup the underlying stubs that will make the mock work
	// ? correctly. Without it UT are failing
	// TODO (@ladislas) - review mocks/stubs to remove the need of the object, replace with setup/teardown functions
	mock::EventQueue _ {};
};

TEST_F(CoreEventQueueTest, initialisation)
{
	ASSERT_NE(&event_queue, nullptr);
}

TEST_F(CoreEventQueueTest, dispatchForever)
{
	event_queue.dispatch_forever();
}

TEST_F(CoreEventQueueTest, call)
{
	MockFunction<void(void)> mock;
	EXPECT_CALL(mock, Call()).Times(1);

	event_queue.dispatch_forever();

	event_queue.call(mock.AsStdFunction());
}

TEST_F(CoreEventQueueTest, callEvery)
{
	MockFunction<void(void)> mock;
	EXPECT_CALL(mock, Call()).Times(1);

	event_queue.dispatch_forever();

	event_queue.call_every(2s, mock.AsStdFunction());
}
