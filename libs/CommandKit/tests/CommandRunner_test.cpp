// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CommandRunner.h"

#include "./mocks/Command.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

class CommandRunnerTest : public testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	stub::EventLoopKit event_loop {};
	CommandRunner cmdrunner {event_loop};
};

TEST_F(CommandRunnerTest, initialization)
{
	EXPECT_NE(&cmdrunner, nullptr);
}

TEST_F(CommandRunnerTest, executeCommandsPushOnce)
{
	auto cmd1 = mock::Command {0x42};
	auto cmd2 = mock::Command {0x43};
	auto cmd3 = mock::Command {0x44};

	auto cmds = std::to_array<interface::Command *>({&cmd1, &cmd2, &cmd3});

	cmdrunner.registerCommand(cmds);

	EXPECT_CALL(cmd1, HasBeenCalled).Times(1);
	EXPECT_CALL(cmd2, HasBeenCalled).Times(1);
	EXPECT_CALL(cmd3, HasBeenCalled).Times(1);

	// clang-format off
	auto data = std::to_array<uint8_t>({
		0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91,
		0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x43, 0x12, 0x02, 0x00, 0x7F, 0xFF, 0x92,
		0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x44, 0x13, 0x02, 0x00, 0x7F, 0xFF, 0x93,
	});
	// clang-format on

	cmdrunner.push(data);
}

TEST_F(CommandRunnerTest, executeCommandsPushMultiple)
{
	auto cmd1 = mock::Command {0x42};
	auto cmd2 = mock::Command {0x43};
	auto cmd3 = mock::Command {0x44};

	auto cmds = std::to_array<interface::Command *>({&cmd1, &cmd2, &cmd3});

	cmdrunner.registerCommand(cmds);

	EXPECT_CALL(cmd1, HasBeenCalled).Times(1);
	EXPECT_CALL(cmd2, HasBeenCalled).Times(1);
	EXPECT_CALL(cmd3, HasBeenCalled).Times(1);

	auto data1 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91});
	auto data2 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x43, 0x12, 0x02, 0x00, 0x7F, 0xFF, 0x92});
	auto data3 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x44, 0x13, 0x02, 0x00, 0x7F, 0xFF, 0x93});

	cmdrunner.push(data1);
	cmdrunner.push(data2);
	cmdrunner.push(data3);
}
