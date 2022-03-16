// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CommandKit.h"

#include "./mocks/Command.h"
#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

// Test list
//  - push data
//  - find start block (pattern)
//  - store in command buffer
//  - do commands

using namespace leka;

class CommandKitTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		logger::set_sink_function([](const char *str, size_t size) {
			auto spy_sink_output = std::string {str};
			std::cout << spy_sink_output;
		});
	}	// void TearDown() override {}

	CommandKit cmdkit {};
};

TEST_F(CommandKitTest, initialization)
{
	EXPECT_NE(&cmdkit, nullptr);
}

TEST_F(CommandKitTest, pushData)
{
	auto data = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 6, 1, 2, 3, 4, 5, 6});

	cmdkit.push(data);
}

TEST_F(CommandKitTest, registerZeroCommand)
{
	EXPECT_EQ(0, cmdkit.size());
}

TEST_F(CommandKitTest, registerOneCommand)
{
	auto cmd = mock::Command {0x01};

	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	EXPECT_EQ(1, cmdkit.size());
}

TEST_F(CommandKitTest, pushDataGood)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	auto data = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91});

	EXPECT_CALL(cmd, HasBeenCalled).Times(1);

	cmdkit.push(data);
}

TEST_F(CommandKitTest, pushDataBadWrongChecksum)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// ? wrong checksum
	auto data = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x00});

	EXPECT_CALL(cmd, HasBeenCalled).Times(0);

	cmdkit.push(data);
}
