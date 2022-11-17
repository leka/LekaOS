// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./mocks/Command.h"
#include "CommandKit.h"
#include "LogKit.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/leka/EventLoopKit.h"

using namespace leka;

class CommandKitPushTest : public testing::Test
{
  protected:
	void SetUp() override
	{
		logger::set_sink_function([](const char *str, std::size_t size) {
			auto spy_sink_output = std::string {str};
			std::cout << spy_sink_output;
		});
	}	// void TearDown() override {}

	stub::EventLoopKit event_loop {};
	CommandKit cmdkit {event_loop};
};

TEST_F(CommandKitPushTest, pushDataGood)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	auto data = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91});

	EXPECT_CALL(cmd, HasBeenCalled).Times(1);

	cmdkit.push(data);
}

TEST_F(CommandKitPushTest, pushDataGoodMultiple)
{
	auto cmd1 = mock::Command {0x42};
	auto cmd2 = mock::Command {0x43};
	auto cmd3 = mock::Command {0x44};

	auto cmds = std::to_array<interface::Command *>({&cmd1, &cmd2, &cmd3});

	cmdkit.registerCommand(cmds);

	EXPECT_CALL(cmd1, HasBeenCalled).Times(1);
	EXPECT_CALL(cmd2, HasBeenCalled).Times(1);
	EXPECT_CALL(cmd3, HasBeenCalled).Times(1);

	auto data1 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91});
	auto data2 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x43, 0x12, 0x02, 0x00, 0x7F, 0xFF, 0x92});
	auto data3 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x44, 0x13, 0x02, 0x00, 0x7F, 0xFF, 0x93});

	cmdkit.push(data1);
	cmdkit.push(data2);
	cmdkit.push(data3);
}

TEST_F(CommandKitPushTest, pushDataGoodInTwoPackets)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// clang-format off
	auto data1 = std::to_array<uint8_t>({   0x2A, 0x2A, 0x2A, 0x2A, 0x01, /* 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91 */});
	auto data2 = std::to_array<uint8_t>({/* 0x2A, 0x2A, 0x2A, 0x2A, 0x01, */ 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91   });
	// clang-format on

	EXPECT_CALL(cmd, HasBeenCalled).Times(1);

	cmdkit.push(data1);
	cmdkit.push(data2);
}

TEST_F(CommandKitPushTest, pushDataGoodInTwoPacketsStartingWithFramePatternOnly)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// clang-format off
	auto data1 = std::to_array<uint8_t>({   0x2A, 0x2A, 0x2A, 0x2A, /* 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91 */});
	auto data2 = std::to_array<uint8_t>({/* 0x2A, 0x2A, 0x2A, 0x2A, */ 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91   });
	// clang-format on

	EXPECT_CALL(cmd, HasBeenCalled).Times(1);

	cmdkit.push(data1);
	cmdkit.push(data2);
}

TEST_F(CommandKitPushTest, pushDataGoodInTwoPacketsStartingWithHalfFramePatternOnly)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// clang-format off
	auto data1 = std::to_array<uint8_t>({   0x2A, 0x2A, /* 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91 */});
	auto data2 = std::to_array<uint8_t>({/* 0x2A, 0x2A, */ 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91   });
	// clang-format on

	EXPECT_CALL(cmd, HasBeenCalled).Times(1);

	cmdkit.push(data1);
	cmdkit.push(data2);
}

TEST_F(CommandKitPushTest, pushDataGoodInTwoPacketsCuttingCommand)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// clang-format off
	auto data1 = std::to_array<uint8_t>({   0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, /* 0x00, 0x7F, 0xFF, 0x91 */});
	auto data2 = std::to_array<uint8_t>({/* 0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, */ 0x00, 0x7F, 0xFF, 0x91   });
	// clang-format on

	EXPECT_CALL(cmd, HasBeenCalled).Times(0);

	cmdkit.push(data1);
	cmdkit.push(data2);
}

TEST_F(CommandKitPushTest, pushDataGoodButNoEnough)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	auto data = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11});

	EXPECT_CALL(cmd, HasBeenCalled).Times(0);

	cmdkit.push(data);
}

TEST_F(CommandKitPushTest, pushDataBadNoFramePattern)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// ? missing frame pattern
	auto data = std::to_array<uint8_t>({0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x91});

	EXPECT_CALL(cmd, HasBeenCalled).Times(0);

	cmdkit.push(data);
}

TEST_F(CommandKitPushTest, pushDataBadWrongChecksum)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// ? wrong checksum
	auto data = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x00});

	EXPECT_CALL(cmd, HasBeenCalled).Times(0);

	cmdkit.push(data);
}

TEST_F(CommandKitPushTest, pushDataBadWrongCommandId)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// ? wrong command id
	auto data = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x00, 0x11, 0x02, 0x00, 0x7F, 0xFF, 0x00});

	EXPECT_CALL(cmd, HasBeenCalled).Times(0);

	cmdkit.push(data);
}

TEST_F(CommandKitPushTest, pushDataMissingData)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// ? missing data
	auto data = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x02, 0x00, 0x7F});

	EXPECT_CALL(cmd, HasBeenCalled).Times(0);

	cmdkit.push(data);
}

TEST_F(CommandKitPushTest, pushDataGeneric)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	// clang-format off
	auto test_cmd_0         = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x00, 0x00, 0x7F, 0xFF, 0x8F});
	auto test_cmd_1         = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x12, 0x02, 0x00, 0x7F, 0xFF, 0x92});

	auto bad_chcksm         = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x00, 0x00, 0x7F, 0xFF, 0x00});
	auto start_frame_only   = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A,                                               });

	auto bad_missing_data_1 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x00, 0x00, 0x7F, 0xFF,     });
	auto bad_missing_data_2 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x00, 0x00, 0x7F,           });
	auto bad_missing_data_3 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x00, 0x00,                 });
	auto bad_missing_data_4 = std::to_array<uint8_t>({0x2A, 0x2A, 0x2A, 0x2A, 0x01, 0x42, 0x11, 0x00,                       });

	auto random_data        = std::to_array<uint8_t>({0x43, 0x2A, 0xB4, 0xCC, 0x54, 0x00, 0x11, 0x22, 0x9F, 0x00, 0xFF, 0x8F});
	// clang-format on

	auto all = std::to_array<std::span<uint8_t>>({test_cmd_0, random_data, test_cmd_1, bad_chcksm, start_frame_only,
												  random_data, bad_missing_data_1, random_data, bad_missing_data_2,
												  test_cmd_1, bad_missing_data_3, random_data, bad_missing_data_4,
												  random_data, test_cmd_0});

	EXPECT_CALL(cmd, HasBeenCalled).Times(4);

	for (auto i = 0; const auto &data: all) {
		cmdkit.push(data);
	}
}

TEST_F(CommandKitPushTest, pushDataPureRandom)
{
	auto cmd  = mock::Command {0x42};
	auto cmds = std::to_array<interface::Command *>({&cmd});

	cmdkit.registerCommand(cmds);

	auto random_data = std::to_array<uint8_t>({0x43, 0x2A, 0xB4, 0xCC, 0x54, 0x00, 0x11, 0x22, 0x9F, 0x00, 0xFF, 0x8F});

	auto random = std::to_array<std::span<uint8_t>>({random_data, random_data, random_data, random_data, random_data,
													 random_data, random_data, random_data, random_data, random_data});

	EXPECT_CALL(cmd, HasBeenCalled).Times(0);

	for (auto i = 0; const auto &data: random) {
		cmdkit.push(data);
	}
}
