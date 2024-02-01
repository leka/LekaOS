// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBufferedSerial.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stubs/mbed/BufferedSerial.h"

using namespace leka;

using ::testing::MockFunction;

class CoreBufferedSerialTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	CoreBufferedSerial coreserial {NC, NC};
};

TEST_F(CoreBufferedSerialTest, instantiation)
{
	ASSERT_NE(&coreserial, nullptr);

	ASSERT_TRUE(spy_BufferedSerial_getEnableInput());
}

TEST_F(CoreBufferedSerialTest, read)
{
	auto expected_read_values = std::vector<uint8_t> {0x01, 0x02};
	auto actual_read_values	  = std::vector<uint8_t> {};
	actual_read_values.resize(2);
	spy_BufferedSerial_setValuesBeforeRead(expected_read_values);
	spy_BufferedSerial_setValuesBeforeRead(expected_read_values);

	auto *buf_ptr = static_cast<uint8_t *>(actual_read_values.data());

	coreserial.read(buf_ptr, actual_read_values.size());

	EXPECT_EQ(expected_read_values, actual_read_values);
}

TEST_F(CoreBufferedSerialTest, write)
{
	auto expected_written_values = std::vector<uint8_t> {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};

	coreserial.write(expected_written_values.data(), expected_written_values.size());

	EXPECT_EQ(expected_written_values, spy_BufferedSerial_getValuesAfterWrite());
}

TEST_F(CoreBufferedSerialTest, readable)
{
	coreserial.readable();

	// nothing expected
}

TEST_F(CoreBufferedSerialTest, enableInput)
{
	coreserial.enable_input();

	ASSERT_TRUE(spy_BufferedSerial_getEnableInput());
}

TEST_F(CoreBufferedSerialTest, disableInput)
{
	coreserial.disable_input();

	ASSERT_FALSE(spy_BufferedSerial_getEnableInput());
}

TEST_F(CoreBufferedSerialTest, sigio)
{
	auto mock_function = MockFunction<void()> {};

	coreserial.sigio(mock_function.AsStdFunction());

	EXPECT_CALL(mock_function, Call);
	auto on_sigio_callback = spy_BufferedSerial_getSigioCallback();
	on_sigio_callback();
}
