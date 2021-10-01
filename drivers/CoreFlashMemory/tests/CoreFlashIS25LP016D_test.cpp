// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFlashIS25LP016D.h"
#include <lstd_array>

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

	for (int i = 0; i < expected_array.size(); i++) {
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

ACTION_TEMPLATE(getDataTransmissionFormat, HAS_1_TEMPLATE_PARAMS(int, k), AND_1_VALUE_PARAMS(returned_format))
{
	interface::QSPI::DataTransmissionFormat actual_format = std::get<k>(args);

	returned_format->command_width		   = actual_format.command_width;
	returned_format->address_width		   = actual_format.address_width;
	returned_format->address_size		   = actual_format.address_size;
	returned_format->alternate_phase_width = actual_format.alternate_phase_width;
	returned_format->alternate_phase_size  = actual_format.alternate_phase_size;
	returned_format->data_width			   = actual_format.data_width;
	returned_format->dummy_cycles		   = actual_format.dummy_cycles;
}

TEST_F(CoreFlashIS25LP016DTest, instantiation)
{
	ASSERT_NE(&flash_memory_is25lp, nullptr);
}

TEST_F(CoreFlashIS25LP016DTest, spiModeStandard)
{
	auto spi_mode = SPIMode::Standard;

	auto expected_data_transmission_format = interface::QSPI::DataTransmissionFormat {};

	expected_data_transmission_format.command_width			= 1;	// QSPI_CFG_BUS_SINGLE
	expected_data_transmission_format.address_width			= 1;	// QSPI_CFG_BUS_SINGLE
	expected_data_transmission_format.address_size			= 24;	// QSPI_CFG_ADDR_SIZE_24
	expected_data_transmission_format.alternate_phase_width = 1;	// QSPI_CFG_BUS_SINGLE
	expected_data_transmission_format.alternate_phase_size	= 8;	// QSPI_CFG_ALT_SIZE_8
	expected_data_transmission_format.data_width			= 1;	// QSPI_CFG_BUS_SINGLE
	expected_data_transmission_format.dummy_cycles			= 0;

	auto actual_data_transmission_format = interface::QSPI::DataTransmissionFormat {};

	EXPECT_CALL(qspimock, setDataTransmissionFormat)
		.WillOnce(getDataTransmissionFormat<0>(&actual_data_transmission_format));
	EXPECT_CALL(qspimock, setFrequency).Times(1);

	flash_memory_is25lp.setSPIMode(spi_mode);

	ASSERT_EQ(expected_data_transmission_format.command_width, actual_data_transmission_format.command_width);
	ASSERT_EQ(expected_data_transmission_format.address_width, actual_data_transmission_format.address_width);
	ASSERT_EQ(expected_data_transmission_format.address_size, actual_data_transmission_format.address_size);
	ASSERT_EQ(expected_data_transmission_format.alternate_phase_width,
			  actual_data_transmission_format.alternate_phase_width);
	ASSERT_EQ(expected_data_transmission_format.alternate_phase_size,
			  actual_data_transmission_format.alternate_phase_size);
	ASSERT_EQ(expected_data_transmission_format.data_width, actual_data_transmission_format.data_width);
	ASSERT_EQ(expected_data_transmission_format.dummy_cycles, actual_data_transmission_format.dummy_cycles);
}

TEST_F(CoreFlashIS25LP016DTest, spiModeDual)
{
	auto spi_mode = SPIMode::Dual;

	EXPECT_CALL(qspimock, setDataTransmissionFormat).Times(1);
	EXPECT_CALL(qspimock, setFrequency).Times(1);

	flash_memory_is25lp.setSPIMode(spi_mode);
}

TEST_F(CoreFlashIS25LP016DTest, spiModeQuad)
{
	auto spi_mode = SPIMode::Quad;

	EXPECT_CALL(qspimock, setDataTransmissionFormat).Times(1);
	EXPECT_CALL(qspimock, setFrequency).Times(1);

	flash_memory_is25lp.setSPIMode(spi_mode);
}

TEST_F(CoreFlashIS25LP016DTest, spiModeDefault)
{
	auto spi_mode = static_cast<spi_mode_t>(99);

	auto expected_data_transmission_format = interface::QSPI::DataTransmissionFormat {};

	expected_data_transmission_format.command_width			= 1;	// QSPI_CFG_BUS_SINGLE
	expected_data_transmission_format.address_width			= 1;	// QSPI_CFG_BUS_SINGLE
	expected_data_transmission_format.address_size			= 24;	// QSPI_CFG_ADDR_SIZE_24
	expected_data_transmission_format.alternate_phase_width = 1;	// QSPI_CFG_BUS_SINGLE
	expected_data_transmission_format.alternate_phase_size	= 8;	// QSPI_CFG_ALT_SIZE_8
	expected_data_transmission_format.data_width			= 1;	// QSPI_CFG_BUS_SINGLE
	expected_data_transmission_format.dummy_cycles			= 0;

	auto actual_data_transmission_format = interface::QSPI::DataTransmissionFormat {};

	EXPECT_CALL(qspimock, setDataTransmissionFormat)
		.WillOnce(getDataTransmissionFormat<0>(&actual_data_transmission_format));
	EXPECT_CALL(qspimock, setFrequency).Times(1);

	flash_memory_is25lp.setSPIMode(spi_mode);

	ASSERT_EQ(expected_data_transmission_format.command_width, actual_data_transmission_format.command_width);
	ASSERT_EQ(expected_data_transmission_format.address_width, actual_data_transmission_format.address_width);
	ASSERT_EQ(expected_data_transmission_format.address_size, actual_data_transmission_format.address_size);
	ASSERT_EQ(expected_data_transmission_format.alternate_phase_width,
			  actual_data_transmission_format.alternate_phase_width);
	ASSERT_EQ(expected_data_transmission_format.alternate_phase_size,
			  actual_data_transmission_format.alternate_phase_size);
	ASSERT_EQ(expected_data_transmission_format.data_width, actual_data_transmission_format.data_width);
	ASSERT_EQ(expected_data_transmission_format.dummy_cycles, actual_data_transmission_format.dummy_cycles);
}

TEST_F(CoreFlashIS25LP016DTest, readModeNormal)
{
	auto read_mode = ReadMode::Normal;
	std::array<uint8_t, 0x10> buffer {};

	auto expected_command_read = command::read;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(false));
		EXPECT_CALL(qspimock, read(expected_command_read, _, _, _, _)).Times(1);
	}

	flash_memory_is25lp.setReadMode(read_mode);
	flash_memory_is25lp.read(0x00, buffer, 0x10);
}

TEST_F(CoreFlashIS25LP016DTest, readModeFast)
{
	auto read_mode = ReadMode::Fast;
	std::array<uint8_t, 0x10> buffer {};

	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(false));
		EXPECT_CALL(qspimock, read).Times(1);
	}

	flash_memory_is25lp.setReadMode(read_mode);
	flash_memory_is25lp.read(0x00, buffer, 0x10);
}

TEST_F(CoreFlashIS25LP016DTest, readModeDefault)
{
	auto read_mode = static_cast<read_mode_t>(99);
	std::array<uint8_t, 0x10> buffer {};

	auto expected_command_read = command::read;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(false));
		EXPECT_CALL(qspimock, read(expected_command_read, _, _, _, _)).Times(1);
	}

	flash_memory_is25lp.setReadMode(read_mode);
	flash_memory_is25lp.read(0x00, buffer, 0x10);
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
	uint32_t address		   = 0x2A;
	const size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer {};

	size_t expected_bytes_read = bytes_to_read;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(false));
		EXPECT_CALL(qspimock, read(command::read, _, address, compareArray(buffer), bytes_to_read))
			.WillOnce(Return(expected_bytes_read));
	}

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashIS25LP016DTest, readOverSize)
{
	uint32_t address		   = flash::is25lp016d::size;
	const size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer {};

	size_t expected_bytes_read = 0;
	EXPECT_CALL(qspimock, read(command::read, _, _, _, _)).Times(0);

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashIS25LP016DTest, readFailChipIsNotAvailable)
{
	uint32_t address		   = 0x2A;
	const size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer {};

	size_t expected_bytes_read = 0;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(true));
		EXPECT_CALL(qspimock, read(command::read, _, _, _, _)).Times(0);
	}

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashIS25LP016DTest, write)
{
	uint32_t address		  = 0x2A;
	auto buffer				  = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	 // "abcdef"
	const auto bytes_to_write = 5;

	size_t expected_bytes_written	  = bytes_to_write;
	auto returned_buffer_read_written = std::tuple<size_t, size_t> {0x00, 0x01};
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, enableWrite).Times(1);
		EXPECT_CALL(flash_manager_is25lp, writeIsNotEnabled).WillOnce(Return(false));
		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(false));
		EXPECT_CALL(qspimock, write(command::write, _, address, compareArray(buffer), bytes_to_write))
			.WillOnce(Return(expected_bytes_written));
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, bytes_to_write);

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashIS25LP016DTest, writeOverSize)
{
	uint32_t address		  = flash::is25lp016d::size;
	auto buffer				  = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	 // "abcdef"
	const auto bytes_to_write = 5;

	size_t expected_bytes_written = 0;
	EXPECT_CALL(qspimock, write(command::write, _, address, _, _)).Times(0);

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, bytes_to_write);

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashIS25LP016DTest, writeFailNotEnableToWrite)
{
	uint32_t address		  = 0x2A;
	auto buffer				  = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	 // "abcdef"
	const auto bytes_to_write = 5;

	size_t expected_bytes_written = 0;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, enableWrite).Times(1);
		EXPECT_CALL(flash_manager_is25lp, writeIsNotEnabled).WillOnce(Return(true));
		EXPECT_CALL(qspimock, write(command::write, _, _, _, _)).Times(0);
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, bytes_to_write);

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashIS25LP016DTest, writeFailChipIsNotAvailable)
{
	uint32_t address		  = 0x2A;
	auto buffer				  = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	 // "abcdef"
	const auto bytes_to_write = 5;

	size_t expected_bytes_written = 0;
	{
		InSequence seq;

		EXPECT_CALL(flash_manager_is25lp, enableWrite).Times(1);
		EXPECT_CALL(flash_manager_is25lp, writeIsNotEnabled).WillOnce(Return(false));
		EXPECT_CALL(flash_manager_is25lp, waitForChipAvailable).Times(1);
		EXPECT_CALL(flash_manager_is25lp, chipIsNotAvailable).WillOnce(Return(true));
		EXPECT_CALL(qspimock, write(command::write, _, _, _, _)).Times(0);
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, bytes_to_write);

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashIS25LP016DTest, erase)
{
	EXPECT_CALL(flash_manager_is25lp, erase).Times(1);

	flash_memory_is25lp.erase();
}
