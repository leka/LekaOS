// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreFlashMemoryIS25LP016D.h"
#include <lstd_array>

#include "gtest/gtest.h"
#include "mocks/leka/QSPI.h"

using namespace leka;

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::InSequence;
using ::testing::Return;

MATCHER_P(setArray, expected_array, "")
{
	if (arg.size() < expected_array.size()) {
		return false;
	}

	for (int i = 0; i < expected_array.size(); i++) {
		arg[i] = expected_array[i];
	}
	return true;
}

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

class CoreFlashMemoryIS25LP016DTest : public ::testing::Test
{
  protected:
	CoreFlashMemoryIS25LP016DTest() : flash_memory_is25lp(qspimock) {}

	// void SetUp() override {}
	// void TearDown() override {}

	void MOCK_FUNCTION_get_status_register(uint8_t expected_value, int times)
	{
		auto returned_value = lstd::to_array({expected_value});

		EXPECT_CALL(qspimock,
					sendCommand(flash_memory::is25lp016d::command::read_status, _, _, _, setArray(returned_value), _))
			.Times(times);
	}

	void MOCK_FUNCTION_chip_available(int times = 1) { MOCK_FUNCTION_get_status_register(0x00, times); }
	void MOCK_FUNCTION_chip_not_available(int times = 1) { MOCK_FUNCTION_get_status_register(0x01, times); }
	void MOCK_FUNCTION_wait_chip_available_and_is_available() { MOCK_FUNCTION_chip_available(2); }
	void MOCK_FUNCTION_wait_chip_available_and_is_not_available(int times = 1)
	{
		{
			InSequence seq;
			for (int i = 0; i < times; i++) {
				MOCK_FUNCTION_chip_available();
				MOCK_FUNCTION_chip_not_available();
			}
		}
	}

	void MOCK_FUNCTION_write_enabled() { MOCK_FUNCTION_get_status_register(0x02, 1); }
	void MOCK_FUNCTION_write_not_enabled() { MOCK_FUNCTION_get_status_register(0x00, 1); }
	void MOCK_FUNCTION_enable_write()
	{
		{
			InSequence seq;

			MOCK_FUNCTION_wait_chip_available_and_is_available();
			EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::write_enable, _, _, _, _, _)).Times(1);
			MOCK_FUNCTION_wait_chip_available_and_is_available();
			MOCK_FUNCTION_write_enabled();
		}
	}
	void MOCK_FUNCTION_enable_write_fail()
	{
		{
			InSequence seq;

			MOCK_FUNCTION_wait_chip_available_and_is_available();
			EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::write_enable, _, _, _, _, _)).Times(1);
			MOCK_FUNCTION_wait_chip_available_and_is_available();
			MOCK_FUNCTION_write_not_enabled();
		}
	}

	mock::QSPI qspimock;
	CoreFlashMemoryIS25LP016D flash_memory_is25lp;
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

TEST_F(CoreFlashMemoryIS25LP016DTest, instantiation)
{
	ASSERT_NE(&flash_memory_is25lp, nullptr);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, spiModeStandard)
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

TEST_F(CoreFlashMemoryIS25LP016DTest, spiModeDual)
{
	auto spi_mode = SPIMode::Dual;

	EXPECT_CALL(qspimock, setDataTransmissionFormat).Times(1);
	EXPECT_CALL(qspimock, setFrequency).Times(1);

	flash_memory_is25lp.setSPIMode(spi_mode);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, spiModeQuad)
{
	auto spi_mode = SPIMode::Quad;

	EXPECT_CALL(qspimock, setDataTransmissionFormat).Times(1);
	EXPECT_CALL(qspimock, setFrequency).Times(1);

	flash_memory_is25lp.setSPIMode(spi_mode);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, spiModeDefault)
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

TEST_F(CoreFlashMemoryIS25LP016DTest, readModeNormal)
{
	auto read_mode = ReadMode::Normal;
	std::array<uint8_t, 10> buffer;

	auto expected_command_read = flash_memory::is25lp016d::command::read;
	EXPECT_CALL(qspimock, read(expected_command_read, _, _, _, _)).Times(1);
	MOCK_FUNCTION_wait_chip_available_and_is_available();

	flash_memory_is25lp.setReadMode(read_mode);
	flash_memory_is25lp.read(0x00, buffer, std::size(buffer));
}

TEST_F(CoreFlashMemoryIS25LP016DTest, readModeFast)
{
	auto read_mode = ReadMode::Fast;
	std::array<uint8_t, 10> buffer;

	EXPECT_CALL(qspimock, read).Times(1);
	MOCK_FUNCTION_wait_chip_available_and_is_available();

	flash_memory_is25lp.setReadMode(read_mode);
	flash_memory_is25lp.read(0x00, buffer, std::size(buffer));
}

TEST_F(CoreFlashMemoryIS25LP016DTest, readModeDefault)
{
	auto read_mode = static_cast<read_mode_t>(99);
	std::array<uint8_t, 10> buffer;

	auto expected_command_read = flash_memory::is25lp016d::command::read;
	EXPECT_CALL(qspimock, read(expected_command_read, _, _, _, _)).Times(1);
	MOCK_FUNCTION_wait_chip_available_and_is_available();

	flash_memory_is25lp.setReadMode(read_mode);
	flash_memory_is25lp.read(0x00, buffer, std::size(buffer));
}

TEST_F(CoreFlashMemoryIS25LP016DTest, getSize)
{
	auto expected_size = flash_memory::is25lp016d::size;
	auto actual_size   = flash_memory_is25lp.getSize();

	ASSERT_EQ(expected_size, actual_size);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, waitForChipAvailableAndIsNotAvailable)
{
	{
		InSequence seq;

		MOCK_FUNCTION_chip_not_available();
		MOCK_FUNCTION_chip_available();
		MOCK_FUNCTION_chip_not_available();
	}

	flash_memory_is25lp.reset();
}

TEST_F(CoreFlashMemoryIS25LP016DTest, reset)
{
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::reset_enable, _, _, _, _, _)).Times(1);
		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::reset, _, _, _, _, _)).Times(1);
	}

	flash_memory_is25lp.reset();
}

TEST_F(CoreFlashMemoryIS25LP016DTest, resetFailBeforeResetEnable)
{
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::reset_enable, _, _, _, _, _)).Times(0);
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::reset, _, _, _, _, _)).Times(0);
	}

	flash_memory_is25lp.reset();
}

TEST_F(CoreFlashMemoryIS25LP016DTest, resetFailBeforeReset)
{
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::reset_enable, _, _, _, _, _)).Times(1);
		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::reset, _, _, _, _, _)).Times(0);
	}

	flash_memory_is25lp.reset();
}

TEST_F(CoreFlashMemoryIS25LP016DTest, enableWrite)
{
	auto buffer		 = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	uint32_t address = 0x2A;
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::write_enable, _, _, _, _, _)).Times(1);
		MOCK_FUNCTION_wait_chip_available_and_is_available();
		MOCK_FUNCTION_write_enabled();

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, write).Times(1);
	}

	flash_memory_is25lp.write(address, buffer, std::size(buffer));
}

TEST_F(CoreFlashMemoryIS25LP016DTest, enableWriteFailChipIsNotAvailableBeforeSendingCommand)
{
	auto buffer		 = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	uint32_t address = 0x2A;

	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::write_enable, _, _, _, _, _)).Times(0);
		MOCK_FUNCTION_wait_chip_available_and_is_available();
		MOCK_FUNCTION_write_not_enabled();

		EXPECT_CALL(qspimock, write).Times(0);
	}

	flash_memory_is25lp.write(address, buffer, std::size(buffer));
}

TEST_F(CoreFlashMemoryIS25LP016DTest, enableWriteFailChipIsNotAvilableBeforeReadingStatusRegister)
{
	auto buffer		 = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	uint32_t address = 0x2A;

	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::write_enable, _, _, _, _, _)).Times(1);
		MOCK_FUNCTION_wait_chip_available_and_is_not_available(2);

		EXPECT_CALL(qspimock, write).Times(0);
	}

	flash_memory_is25lp.write(address, buffer, std::size(buffer));
}

TEST_F(CoreFlashMemoryIS25LP016DTest, read)
{
	const size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer {};
	uint32_t address = 0x2A;

	size_t expected_bytes_read = bytes_to_read;
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, read(flash_memory::is25lp016d::command::read, _, address, compareArray(buffer), _))
			.WillOnce(Return(expected_bytes_read));
	}

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, readOverSize)
{
	const size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer;
	uint32_t address = flash_memory::is25lp016d::size;

	size_t expected_bytes_read = 0;
	EXPECT_CALL(qspimock, read(flash_memory::is25lp016d::command::read, _, address, _, _)).Times(0);

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, readFailChipIsNotAvailable)
{
	const size_t bytes_to_read = 0x10;
	std::array<uint8_t, bytes_to_read> buffer {};
	uint32_t address = 0x2A;

	size_t expected_bytes_read = 0;
	{
		InSequence seq;

		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, read(flash_memory::is25lp016d::command::read, _, _, _, _)).Times(0);
	}

	auto actual_bytes_read = flash_memory_is25lp.read(address, buffer, bytes_to_read);

	ASSERT_EQ(expected_bytes_read, actual_bytes_read);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, write)
{
	auto buffer		 = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	uint32_t address = 0x2A;

	size_t expected_bytes_written	  = 0x10;
	auto returned_buffer_read_written = std::tuple<size_t, size_t> {0x00, 0x01};
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write();
		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, write(flash_memory::is25lp016d::command::write, _, address, compareArray(buffer), _))
			.WillOnce(Return(expected_bytes_written));
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, std::size(buffer));

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, writeOverSize)
{
	auto buffer		 = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	uint32_t address = flash_memory::is25lp016d::size;

	size_t expected_bytes_written = 0;
	EXPECT_CALL(qspimock, write(flash_memory::is25lp016d::command::write, _, address, _, _)).Times(0);

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, std::size(buffer));

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, writeFailNotEnableToWrite)
{
	auto buffer		 = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	uint32_t address = 0x2A;

	size_t expected_bytes_written = 0;
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write_fail();
		EXPECT_CALL(qspimock, write(flash_memory::is25lp016d::command::write, _, _, _, _)).Times(0);
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, std::size(buffer));

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, writeFailChipIsNotAvailable)
{
	auto buffer		 = lstd::to_array<uint8_t>({0x61, 0x62, 0x63, 0x64, 0x65, 0x66});	// "abcdef"
	uint32_t address = 0x2A;

	size_t expected_bytes_written = 0;
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write();
		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, write(flash_memory::is25lp016d::command::write, _, _, _, _)).Times(0);
	}

	auto actual_bytes_write = flash_memory_is25lp.write(address, buffer, std::size(buffer));

	ASSERT_EQ(expected_bytes_written, actual_bytes_write);
}

TEST_F(CoreFlashMemoryIS25LP016DTest, erase)
{
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write();
		MOCK_FUNCTION_wait_chip_available_and_is_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::erase_chip, _, _, _, _, _)).Times(1);
	}

	flash_memory_is25lp.erase();
}

TEST_F(CoreFlashMemoryIS25LP016DTest, eraseFailNotEnableWrite)
{
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write_fail();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::erase_chip, _, _, _, _, _)).Times(0);
	}

	flash_memory_is25lp.erase();
}

TEST_F(CoreFlashMemoryIS25LP016DTest, eraseFailChipNotAvailable)
{
	{
		InSequence seq;

		MOCK_FUNCTION_enable_write();
		MOCK_FUNCTION_wait_chip_available_and_is_not_available();
		EXPECT_CALL(qspimock, sendCommand(flash_memory::is25lp016d::command::erase_chip, _, _, _, _, _)).Times(0);
	}

	flash_memory_is25lp.erase();
}
