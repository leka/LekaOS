// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreSPI.h"

#include "gtest/gtest.h"
#include "stubs/mbed/SPI.h"

using namespace leka;

class CoreSPITests : public ::testing::Test
{
  protected:
	CoreSPITests() : corespi(NC, NC, NC, NC) {}

	// void SetUp() override {}
	// void TearDown() override {}

	CoreSPI corespi;
};

TEST_F(CoreSPITests, initialisation)
{
	ASSERT_NE(&corespi, nullptr);
}

TEST_F(CoreSPITests, writeOneByte)
{
	auto expected_written_values = std::vector<uint8_t> {0x01};

	corespi.write(expected_written_values);

	ASSERT_EQ(expected_written_values, spy_spi_getValuesAfterWrite());
}

TEST_F(CoreSPITests, writeTwoBytes)
{
	auto expected_written_values = std::vector<uint8_t> {0x01, 0x02};

	corespi.write(expected_written_values);

	ASSERT_EQ(expected_written_values, spy_spi_getValuesAfterWrite());
}

TEST_F(CoreSPITests, writeFiveBytes)
{
	auto expected_written_values = std::vector<uint8_t> {0x01, 0x02, 0x03, 0x04, 0x05};

	corespi.write(expected_written_values);

	ASSERT_EQ(expected_written_values, spy_spi_getValuesAfterWrite());
}

TEST_F(CoreSPITests, writeTenBytes)
{
	auto expected_written_values = std::vector<uint8_t> {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};

	corespi.write(expected_written_values);

	ASSERT_EQ(expected_written_values, spy_spi_getValuesAfterWrite());
}

TEST_F(CoreSPITests, writeReturnSize)
{
	auto expected_written_values = std::vector<uint8_t> {0x01, 0x02, 0x03, 0x04, 0x05};

	auto size = corespi.write(expected_written_values);

	ASSERT_EQ(std::size(expected_written_values), size);
}
