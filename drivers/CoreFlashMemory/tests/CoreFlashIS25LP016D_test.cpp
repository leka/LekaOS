// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFlashIS25LP016D.h"
#include <array>

#include "gtest/gtest.h"
#include "mocks/leka/FlashManager.h"
#include "mocks/leka/QSPI.h"

using namespace leka;
using namespace flash::is25lp016d;

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

MATCHER_P(compareArray, expected_array, "")
{
	if (arg.size() != expected_array.size()) {
		return false;
	}

	auto same_content = true;

	for (unsigned i = 0; i < expected_array.size(); i++) {
		same_content &= arg[i] == expected_array[i];
	}
	return same_content;
}

class CoreFlashIS25LP016DTest : public ::testing::Test
{
  protected:
	CoreFlashIS25LP016DTest() : flash_memory_is25lp(qspimock, flash_manager_is25lp) {}

	// void SetUp() override {}
	// void TearDown() override {}

	mock::QSPI qspimock;
	mock::FlashManager flash_manager_is25lp;
	CoreFlashIS25LP016D flash_memory_is25lp;
};

TEST_F(CoreFlashIS25LP016DTest, instantiation)
{
	ASSERT_NE(&flash_memory_is25lp, nullptr);
}

TEST_F(CoreFlashIS25LP016DTest, getSize)
{
	auto expected_size = flash::is25lp016d::size;
	auto actual_size   = flash_memory_is25lp.getSize();

	ASSERT_EQ(expected_size, actual_size);
}

TEST_F(CoreFlashIS25LP016DTest, reset)
{
	EXPECT_CALL(flash_manager_is25lp, reset).Times(1);

	flash_memory_is25lp.reset();
}

TEST_F(CoreFlashIS25LP016DTest, read)
{
	uint32_t address				= 0x2A;
	const std::size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer {};

	size_t expected_bytes_read = bytes_to_read;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(false));
		EXPECT_CALL(qspimock, read(command::read, address, compareArray(buffer), bytes_to_read))
			.WillOnce(Return(expected_bytes_read));
	}

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashIS25LP016DTest, readOverSize)
{
	uint32_t address				= flash::is25lp016d::size;
	const std::size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer {};

	size_t expected_bytes_read = 0;
	EXPECT_CALL(qspimock, read(command::read, _, _, _)).Times(0);

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashIS25LP016DTest, readFailChipIsNotAvailable)
{
	uint32_t address				= 0x2A;
	const std::size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer {};

	size_t expected_bytes_read = 0;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(true));
		EXPECT_CALL(qspimock, read(command::read, _, _, _)).Times(0);
	}

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashIS25LP016DTest, write)
{
	uint32_t address		  = 0x2A;
	auto buffer				  = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	const auto bytes_to_write = 5;

	size_t expected_bytes_written	  = bytes_to_write;
	auto returned_buffer_read_written = std::tuple<size_t, std::size_t> {0x00, 0x01};
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, enableWrite).Times(1);
		EXPECT_CALL(flash_manager_is25lp, writeIsNotEnabled).WillOnce(Return(false));
		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(false));
		EXPECT_CALL(qspimock, write(command::write, address, compareArray(buffer), bytes_to_write))
			.WillOnce(Return(expected_bytes_written));
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, bytes_to_write);

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashIS25LP016DTest, writeOverSize)
{
	uint32_t address		  = flash::is25lp016d::size;
	auto buffer				  = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	const auto bytes_to_write = 5;

	size_t expected_bytes_written = 0;
	EXPECT_CALL(qspimock, write(command::write, address, _, _)).Times(0);

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, bytes_to_write);

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashIS25LP016DTest, writeFailNotEnableToWrite)
{
	uint32_t address		  = 0x2A;
	auto buffer				  = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	const auto bytes_to_write = 5;

	size_t expected_bytes_written = 0;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, enableWrite).Times(1);
		EXPECT_CALL(flash_manager_is25lp, writeIsNotEnabled).WillOnce(Return(true));
		EXPECT_CALL(qspimock, write(command::write, _, _, _)).Times(0);
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, bytes_to_write);

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashIS25LP016DTest, writeFailChipIsNotAvailable)
{
	uint32_t address		  = 0x2A;
	auto buffer				  = std::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	const auto bytes_to_write = 5;

	size_t expected_bytes_written = 0;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, enableWrite).Times(1);
		EXPECT_CALL(flash_manager_is25lp, writeIsNotEnabled).WillOnce(Return(false));
		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(true));
		EXPECT_CALL(qspimock, write(command::write, _, _, _)).Times(0);
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, bytes_to_write);

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashIS25LP016DTest, erase)
{
	EXPECT_CALL(flash_manager_is25lp, erase).Times(1);

	flash_memory_is25lp.erase();
}
