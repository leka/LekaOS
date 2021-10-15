// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQSPI.h"

#include "gtest/gtest.h"
#include "stubs/mbed/QSPI.h"

using namespace leka;

TEST(CoreQSPITest, initialisation)
{
	auto coreqspi = CoreQSPI {};

	ASSERT_NE(&coreqspi, nullptr);
}

TEST(CoreQSPITest, initialisationWithparameters)
{
	auto coreqspi = CoreQSPI {NC, NC, NC, NC, NC, NC};

	ASSERT_NE(&coreqspi, nullptr);
}

TEST(CoreQSPITest, setDataTransmissionFormat)
{
	auto coreqspi = CoreQSPI {};

	auto expected_command_width			= QSPI_CFG_BUS_SINGLE;
	auto expected_address_width			= QSPI_CFG_BUS_SINGLE;
	auto expected_address_size			= QSPI_CFG_ADDR_SIZE_24;
	auto expected_alternate_phase_width = QSPI_CFG_BUS_SINGLE;
	auto expected_alternate_phase_size	= QSPI_CFG_ALT_SIZE_8;
	auto expected_data_width			= QSPI_CFG_BUS_SINGLE;
	auto expected_dummy_cycles			= 0;

	coreqspi.setDataTransmissionFormat();

	ASSERT_EQ(spy_QSPI_command_width, expected_command_width);
	ASSERT_EQ(spy_QSPI_address_width, expected_address_width);
	ASSERT_EQ(spy_QSPI_address_size, expected_address_size);
	ASSERT_EQ(spy_QSPI_alternate_phase_width, expected_alternate_phase_width);
	ASSERT_EQ(spy_QSPI_alternate_phase_size, expected_alternate_phase_size);
	ASSERT_EQ(spy_QSPI_data_width, expected_data_width);
	ASSERT_EQ(spy_QSPI_dummy_cycle, expected_dummy_cycles);
}

TEST(CoreQSPITest, setFrequency)
{
	auto coreqspi = CoreQSPI {};

	coreqspi.setFrequency(133'000'000);

	ASSERT_EQ(spy_QSPI_frequency, 133'000'000);
}

TEST(CoreQSPITest, write)
{
	auto coreqspi = CoreQSPI {};

	uint8_t data[] = "This is a test";

	auto bytes_written = coreqspi.write(0x2A, 0x2B, data, std::size(data));

	ASSERT_EQ(spy_QSPI_command, 0x2A);
	ASSERT_EQ(spy_QSPI_address, 0x2B);
	ASSERT_EQ(spy_QSPI_tx_buffer_length, bytes_written);

	for (int index = 0; index < bytes_written; index++) {
		ASSERT_EQ(spy_QSPI_tx_buffer[index], data[index]);
	}
}

TEST(CoreQSPITest, read)
{
	auto coreqspi = CoreQSPI {};

	constexpr size_t buffer_size = 0x10;
	std::array<uint8_t, buffer_size> buffer;

	auto bytes_read = coreqspi.read(0x2C, 0x2D, buffer, buffer_size);

	ASSERT_EQ(spy_QSPI_command, 0x2C);
	ASSERT_EQ(spy_QSPI_address, 0x2D);
	ASSERT_EQ(spy_QSPI_rx_buffer_length, bytes_read);

	for (int index = 0; index < bytes_read; index++) {
		ASSERT_EQ(spy_QSPI_rx_buffer[index], buffer[index]);
	}
}

TEST(CoreQSPITest, sendCommand)
{
	auto coreqspi = CoreQSPI {};

	uint8_t tx_data[]			  = "Another test";
	constexpr size_t rx_data_size = 0x15;
	std::array<uint8_t, rx_data_size> rx_data;

	auto bytes_written_read = coreqspi.sendCommand(0x2E, 0x2F, tx_data, std::size(tx_data), rx_data, rx_data_size);

	auto bytes_written = std::get<0>(bytes_written_read);
	auto bytes_read	   = std::get<1>(bytes_written_read);

	ASSERT_EQ(spy_QSPI_command, 0x2E);
	ASSERT_EQ(spy_QSPI_address, 0x2F);
	ASSERT_EQ(spy_QSPI_tx_buffer_length, bytes_written);
	ASSERT_EQ(spy_QSPI_rx_buffer_length, bytes_read);

	for (int index = 0; index < bytes_written; index++) {
		ASSERT_EQ(spy_QSPI_tx_buffer[index], tx_data[index]);
	}

	// for (int index = 0; index < rx_length; index++) {
	// 	ASSERT_EQ(spy_QSPI_rx_buffer[index], rx_data[index]);
	// }
}
