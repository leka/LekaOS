// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./mocks/Command.h"
#include "CommandKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

class CommandKitRegisterTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	stub::EventLoopKit event_loop {};
	CommandKit cmdkit {event_loop};
};

TEST_F(CommandKitRegisterTest, registerZeroCommand)
{
	EXPECT_EQ(0, cmdkit.size());
}

TEST_F(CommandKitRegisterTest, registerOneCommand)
{
	auto cmd = mock::Command {0x01};

	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	EXPECT_EQ(1, cmdkit.size());
}

TEST_F(CommandKitRegisterTest, registerMultipleCommands)
{
	auto cmd1 = mock::Command {0x01};
	auto cmd2 = mock::Command {0x02};
	auto cmd3 = mock::Command {0x03};

	auto cmds = std::to_array<interface::Command *>({&cmd1, &cmd2, &cmd3});

	cmdkit.registerCommand(cmds);

	EXPECT_EQ(3, cmdkit.size());
}
