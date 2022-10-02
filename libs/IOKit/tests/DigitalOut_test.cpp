// Leka - LekaOS
// Copyright 2022 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "IOKit/DigitalOut.h"

#include "gtest/gtest.h"
#include "mocks/leka/IOExpander.h"

using namespace leka;
using ::testing::_;

class DigitalOutTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	leka::mock::IOExpander<uint16_t> mockIOExpander {};
	uint16_t pin_number = 0;
};

TEST_F(DigitalOutTest, instantiationDefaultPinType)
{
	EXPECT_CALL(mockIOExpander, setPinAsOutput(pin_number)).Times(1);

	leka::io::expanded::DigitalOut pin {mockIOExpander, pin_number};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalOutTest, instantiationExplicitPinTypeUint16t)
{
	leka::mock::IOExpander<uint16_t> expander {};
	auto pin_num = uint16_t {0};

	EXPECT_CALL(expander, setPinAsOutput(pin_num)).Times(1);

	leka::io::expanded::DigitalOut<uint16_t> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalOutTest, instantiationExplicitPinTypeUint8t)
{
	leka::mock::IOExpander<uint8_t> expander {};
	auto pin_num = uint8_t {0};

	EXPECT_CALL(expander, setPinAsOutput(pin_num)).Times(1);

	leka::io::expanded::DigitalOut<uint8_t> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalOutTest, instantiationExplicitPinTypeEnumDefault)
{
	enum class TestPinNames
	{
		PA_0 = 0,
		PA_1 = 1
	};

	leka::mock::IOExpander<TestPinNames> expander {};
	auto pin_num = TestPinNames::PA_0;

	EXPECT_CALL(expander, setPinAsOutput(pin_num)).Times(1);

	leka::io::expanded::DigitalOut<TestPinNames> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalOutTest, instantiationExplicitPinTypeEnumUint16t)
{
	enum class TestPinNames : uint16_t
	{
		PA_0 = 0,
		PA_1 = 1
	};

	leka::mock::IOExpander<TestPinNames> expander {};
	auto pin_num = TestPinNames::PA_0;

	EXPECT_CALL(expander, setPinAsOutput(pin_num)).Times(1);

	leka::io::expanded::DigitalOut<TestPinNames> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalOutTest, instantiationExplicitPinTypeEnumUint8t)
{
	enum class TestPinNames : uint8_t
	{
		PA_0 = 0,
		PA_1 = 1
	};

	leka::mock::IOExpander<TestPinNames> expander {};
	auto pin_num = TestPinNames::PA_0;

	EXPECT_CALL(expander, setPinAsOutput(pin_num)).Times(1);

	leka::io::expanded::DigitalOut<TestPinNames> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalOutTest, writeHigh)
{
	EXPECT_CALL(mockIOExpander, setPinAsOutput(pin_number)).Times(1);
	EXPECT_CALL(mockIOExpander, writePin(pin_number, 0x01)).Times(1);

	leka::io::expanded::DigitalOut<uint16_t> pin {mockIOExpander, pin_number};

	pin.write(0x01);
}

TEST_F(DigitalOutTest, writeLow)
{
	EXPECT_CALL(mockIOExpander, setPinAsOutput(pin_number)).Times(1);
	EXPECT_CALL(mockIOExpander, writePin(pin_number, 0x00)).Times(1);

	leka::io::expanded::DigitalOut<uint16_t> pin {mockIOExpander, pin_number};

	pin.write(0x00);
}

TEST_F(DigitalOutTest, read)
{
	EXPECT_CALL(mockIOExpander, setPinAsOutput(pin_number)).Times(1);
	EXPECT_CALL(mockIOExpander, readPin(pin_number)).Times(1);

	leka::io::expanded::DigitalOut<uint16_t> pin {mockIOExpander, pin_number};

	auto val = pin.read();
}

TEST_F(DigitalOutTest, isConnected)
{
	EXPECT_CALL(mockIOExpander, setPinAsOutput(pin_number)).Times(1);

	leka::io::expanded::DigitalOut<uint16_t> pin {mockIOExpander, pin_number};

	auto isConnected = pin.is_connected();

	ASSERT_TRUE(isConnected);
}
