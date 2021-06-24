// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CriticalSection.h"
#include <mutex>

#include "gtest/gtest.h"
#include "stubs/mbed/mbed_critical.h"

using namespace leka;

class CriticalSectionTest : public ::testing::Test
{
  protected:
	void SetUp() override
	{
		spy_mbed_critical_enter_calls = 0;
		spy_mbed_critical_exit_calls  = 0;
	}
	// void TearDown() override {}

	CriticalSection cs {};
};

TEST_F(CriticalSectionTest, initialization)
{
	ASSERT_NE(&cs, nullptr);
	ASSERT_FALSE(spy_mbed_critical_enter_was_called());
	ASSERT_FALSE(spy_mbed_critical_exit_was_called());
}

TEST_F(CriticalSectionTest, enterCriticalSection)
{
	cs.lock();

	ASSERT_TRUE(spy_mbed_critical_enter_was_called());
	ASSERT_FALSE(spy_mbed_critical_exit_was_called());
}

TEST_F(CriticalSectionTest, exitCriticalSection)
{
	cs.unlock();

	ASSERT_FALSE(spy_mbed_critical_enter_was_called());
	ASSERT_TRUE(spy_mbed_critical_exit_was_called());
}

TEST_F(CriticalSectionTest, raii)
{
	{
		const std::lock_guard<CriticalSection> lock(cs);
	}

	ASSERT_TRUE(spy_mbed_critical_enter_was_called());
	ASSERT_TRUE(spy_mbed_critical_exit_was_called());
}
