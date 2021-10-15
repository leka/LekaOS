// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

#include "gtest/gtest.h"
#include "mocks/leka/File.h"
#include "mocks/leka/FlashMemory.h"

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
	mock::File file;
	FirmwareKit firmwarekit;
};

TEST_F(FirmwareKitTest, instantiation)
{
	ASSERT_NE(&firmwarekit, nullptr);
}

TEST_F(FirmwareKitTest, loadUpdateCheckPackets)
{
	auto expected_packet_size				 = 0x100;
	auto actual_read_packet_size_first_loop	 = 0x80;
	auto actual_read_packet_size_second_loop = 0;

	{
		InSequence seq;

		EXPECT_CALL(file, read(_, expected_packet_size)).WillOnce(Return(actual_read_packet_size_first_loop));
		EXPECT_CALL(flash_memory, write(_, _, actual_read_packet_size_first_loop)).Times(1);

		EXPECT_CALL(file, read(_, expected_packet_size)).WillOnce(Return(actual_read_packet_size_second_loop));
		EXPECT_CALL(flash_memory, write(_, _, actual_read_packet_size_second_loop)).Times(0);

		EXPECT_CALL(file, close).Times(1);
	}

	firmwarekit.loadUpdate(file);
}

TEST_F(FirmwareKitTest, loadUpdateCheckAddress)
{
	auto expected_packet_size				= 0x100;
	auto expected_adress					= 0x0;
	auto actual_read_packet_size_first_loop = 0x80;

	{
		InSequence seq;

		EXPECT_CALL(file, read(_, _)).WillOnce(Return(actual_read_packet_size_first_loop));
		EXPECT_CALL(flash_memory, write(expected_adress, _, _)).Times(1);
		expected_adress += actual_read_packet_size_first_loop;

		EXPECT_CALL(file, read(_, _)).WillOnce(Return(0));
		EXPECT_CALL(flash_memory, write(expected_adress, _, _)).Times(0);

		EXPECT_CALL(file, close).Times(1);
	}

	firmwarekit.loadUpdate(file);
}
