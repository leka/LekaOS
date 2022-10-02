// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMCU.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(CoreMCUTest, initialisation)
{
	auto coremcu = CoreMCU {};

	EXPECT_NE(&coremcu, nullptr);
}

TEST(CoreMCUTest, getID)
{
	auto coremcu = CoreMCU {};

	auto not_expected = MCUID {1, 2, 3};

	// ? This cannot be tested as the memory address is not accessible
	// ? on the host platform
	// auto actual = coremcu.getID();
}
