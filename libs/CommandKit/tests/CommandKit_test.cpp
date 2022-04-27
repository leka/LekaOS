// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CommandKit.h"

#include "./mocks/Command.h"
#include "LogKit.h"
#include "gtest/gtest.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

TEST(CommandKitTest, initialization)
{
	stub::EventLoopKit event_loop {};
	CommandKit cmdkit {event_loop};

	EXPECT_NE(&cmdkit, nullptr);
}
