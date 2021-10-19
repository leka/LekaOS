// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

#include "gtest/gtest.h"
// #include "mocks/leka/File.h"
#include "mocks/leka/FlashMemory.h"
// #include "stubs/mbed/File.h"

using namespace leka;

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

class FirmwareKitTest : public ::testing::Test
{
  protected:
	FirmwareKitTest() : firmwarekit(flash_memory) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::FlashMemory flash_memory;
	// mock::File file;
	FirmwareKit firmwarekit;

	// void MOCK_FUNCTION_end_of_file_reached() { EXPECT_CALL(file, read(_, _)).WillOnce(Return(0)); }
};

MATCHER_P(compareStrings, expected_string, "")
{
	bool same_content = true;

	for (int i = 0; i < std::size(expected_string); i++) {
		same_content &= expected_string[i] == arg[i];
	}

	return same_content;
}

TEST_F(FirmwareKitTest, instantiation)
{
	ASSERT_NE(&firmwarekit, nullptr);
}

// TEST_F(FirmwareKitTest, loadUpdateCheckPackets)
// {
// 	auto expected_load_update	 = true;
// 	auto expected_packet_size	 = 256;
// 	auto actual_some_packet_read = 200;
// 	std::string expected_path	 = "/fs/some_path_to_update.bin";
// 	std::string expected_mode	 = "r";

// 	{
// 		InSequence seq;

// 		EXPECT_CALL(file, open(compareStrings(expected_path), compareStrings(expected_mode))).WillOnce(Return(true));

// 		EXPECT_CALL(file, read(_, expected_packet_size)).WillOnce(Return(actual_some_packet_read));
// 		EXPECT_CALL(flash_memory, write(_, _, actual_some_packet_read)).Times(1);

// 		MOCK_FUNCTION_end_of_file_reached();

// 		EXPECT_CALL(file, close).Times(1);
// 	}

// 	auto actual_load_update = firmwarekit.loadUpdate(file, expected_path.data());

// 	ASSERT_EQ(actual_load_update, expected_load_update);
// }

// TEST_F(FirmwareKitTest, loadUpdateCheckAddress)
// {
// 	auto expected_load_update	 = true;
// 	auto expected_adress		 = 0x0;
// 	auto actual_some_packet_read = 200;
// 	std::string expected_path	 = "/fs/some_path_to_update.bin";
// 	std::string expected_mode	 = "r";

// 	{
// 		InSequence seq;

// 		EXPECT_CALL(file, open(compareStrings(expected_path), compareStrings(expected_mode))).WillOnce(Return(true));

// 		EXPECT_CALL(file, read(_, _)).WillOnce(Return(actual_some_packet_read));
// 		EXPECT_CALL(flash_memory, write(expected_adress, _, _)).Times(1);
// 		expected_adress += actual_some_packet_read;

// 		EXPECT_CALL(file, read(_, _)).WillOnce(Return(actual_some_packet_read));
// 		EXPECT_CALL(flash_memory, write(expected_adress, _, _)).Times(1);

// 		MOCK_FUNCTION_end_of_file_reached();

// 		EXPECT_CALL(file, close).Times(1);
// 	}

// 	auto actual_load_update = firmwarekit.loadUpdate(file, expected_path.data());

// 	ASSERT_EQ(actual_load_update, expected_load_update);
// }

// TEST_F(FirmwareKitTest, loadUpdateFileNotOpened)
// {
// 	auto expected_load_update = false;
// 	std::string expected_path = "/fs/some_path_to_update.bin";
// 	std::string expected_mode = "r";
// 	{
// 		InSequence seq;

// 		EXPECT_CALL(file, open(compareStrings(expected_path), compareStrings(expected_mode))).WillOnce(Return(false));

// 		EXPECT_CALL(file, read(_, _)).Times(0);
// 		EXPECT_CALL(flash_memory, write).Times(0);
// 	}

// 	auto actual_load_update = firmwarekit.loadUpdate(file, expected_path.data());

// 	ASSERT_EQ(actual_load_update, expected_load_update);
// }
