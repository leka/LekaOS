// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreEventFlags.h"

#include "gtest/gtest.h"

using namespace leka;

class CoreEventFlagsTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	CoreEventFlags flags {};
};

TEST_F(CoreEventFlagsTest, initialisation)
{
	ASSERT_NE(&flags, nullptr);
}

TEST_F(CoreEventFlagsTest, setSuccess)
{
	CoreEventFlags::eventflags_t f1 = 42;

	auto ret = flags.set(f1);

	EXPECT_EQ(f1, ret);
}

TEST_F(CoreEventFlagsTest, get)
{
	CoreEventFlags::eventflags_t f1 = 42;

	std::ignore = flags.set(f1);

	auto ret_f1 = flags.get();

	// ? Should EXPECT_EQ(f1, ret_f1);
	// ? But stub doesn't `get` anything
}

TEST_F(CoreEventFlagsTest, clearSuccess)
{
	CoreEventFlags::eventflags_t f1 = 42;

	std::ignore = flags.set(f1);

	auto ret = flags.clear(f1);

	EXPECT_EQ(f1, ret);
}

TEST_F(CoreEventFlagsTest, waitAny)
{
	CoreEventFlags::eventflags_t f1 = 42;

	std::ignore = flags.set(f1);

	flags.wait_any(f1);

	// ? Nothing to expect
}
