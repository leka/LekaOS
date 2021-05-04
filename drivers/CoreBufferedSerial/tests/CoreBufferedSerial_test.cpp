// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBufferedSerial.h"

#include "gtest/gtest.h"
#include "stub_BufferedSerial.h"

using namespace leka;

class CoreBufferedSerialTests : public ::testing::Test
{
  protected:
	CoreBufferedSerialTests() : coreBufferedSerial(interface) {}

	void SetUp() override
	{
		expected_read_values	= {};
		actual_read_values		= {};
		expected_written_values = {};
	}
	// void TearDown() override {}

	mbed::BufferedSerial interface {NC, NC};
	CoreBufferedSerial coreBufferedSerial;

	std::vector<uint8_t> expected_read_values;
	std::vector<uint8_t> actual_read_values;
	std::vector<uint8_t> expected_written_values;
};

TEST_F(CoreBufferedSerialTests, initialisation)
{
	ASSERT_NE(&coreBufferedSerial, nullptr);
}
