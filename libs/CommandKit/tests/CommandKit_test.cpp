// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CommandKit.h"

#include "./mocks/Command.h"
#include "LogKit.h"
#include "gtest/gtest.h"

using namespace leka;

TEST(CommandKitTest, initialization)
{
	CommandKit cmdkit {};
	EXPECT_NE(&cmdkit, nullptr);
}
