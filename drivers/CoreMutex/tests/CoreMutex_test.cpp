// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMutex.h"

#include "gtest/gtest.h"

using namespace leka;

class CoreMutexTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	CoreMutex cm {};
};

TEST_F(CoreMutexTest, initialization)
{
	ASSERT_NE(&cm, nullptr);
}

TEST_F(CoreMutexTest, lock)
{
	cm.lock();
}

TEST_F(CoreMutexTest, lockUnlock)
{
	cm.lock();
	cm.unlock();
}

TEST_F(CoreMutexTest, tryLock)
{
	auto locked = cm.try_lock();

	EXPECT_TRUE(locked);
}
