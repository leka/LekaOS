// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#include "IOKit/DigitalIn.h"

#include "gtest/gtest.h"
#include "mocks/leka/IOExpander.h"

using namespace leka;
using ::testing::_;

class DigitalInTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	leka::mock::IOExpander<uint16_t> mockIOExpander {};
	uint16_t pin_number = 0;
};

TEST_F(DigitalInTest, instantiationDefaultPinType)
{
	EXPECT_CALL(mockIOExpander, setPinAsInput(pin_number)).Times(1);

	leka::io::expanded::DigitalIn pin {mockIOExpander, pin_number};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalInTest, instantiationExplicitPinTypeUint16t)
{
	leka::mock::IOExpander<uint16_t> expander {};
	auto pin_num = uint16_t {0};

	EXPECT_CALL(expander, setPinAsInput(pin_num)).Times(1);

	leka::io::expanded::DigitalIn<uint16_t> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalInTest, instantiationExplicitPinTypeUint8t)
{
	leka::mock::IOExpander<uint8_t> expander {};
	auto pin_num = uint8_t {0};

	EXPECT_CALL(expander, setPinAsInput(pin_num)).Times(1);

	leka::io::expanded::DigitalIn<uint8_t> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalInTest, instantiationExplicitPinTypeEnumDefault)
{
	enum class TestPinNames
	{
		PA_0 = 0,
		PA_1 = 1
	};

	leka::mock::IOExpander<TestPinNames> expander {};
	auto pin_num = TestPinNames::PA_0;

	EXPECT_CALL(expander, setPinAsInput(pin_num)).Times(1);

	leka::io::expanded::DigitalIn<TestPinNames> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalInTest, instantiationExplicitPinTypeEnumUint16t)
{
	enum class TestPinNames : uint16_t
	{
		PA_0 = 0,
		PA_1 = 1
	};

	leka::mock::IOExpander<TestPinNames> expander {};
	auto pin_num = TestPinNames::PA_0;

	EXPECT_CALL(expander, setPinAsInput(pin_num)).Times(1);

	leka::io::expanded::DigitalIn<TestPinNames> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalInTest, instantiationExplicitPinTypeEnumUint8t)
{
	enum class TestPinNames : uint8_t
	{
		PA_0 = 0,
		PA_1 = 1
	};

	leka::mock::IOExpander<TestPinNames> expander {};
	auto pin_num = TestPinNames::PA_0;

	EXPECT_CALL(expander, setPinAsInput(pin_num)).Times(1);

	leka::io::expanded::DigitalIn<TestPinNames> pin {expander, pin_num};

	ASSERT_NE(&pin, nullptr);
}

TEST_F(DigitalInTest, setModePullNone)
{
	EXPECT_CALL(mockIOExpander, setPinAsInput(pin_number)).Times(1);
	EXPECT_CALL(mockIOExpander, setModeForPin(pin_number, PullNone)).Times(1);

	leka::io::expanded::DigitalIn<uint16_t> pin {mockIOExpander, pin_number};

	pin.mode(PullNone);
}

TEST_F(DigitalInTest, setModePullUp)
{
	EXPECT_CALL(mockIOExpander, setPinAsInput(pin_number)).Times(1);
	EXPECT_CALL(mockIOExpander, setModeForPin(pin_number, PullUp)).Times(1);

	leka::io::expanded::DigitalIn<uint16_t> pin {mockIOExpander, pin_number};

	pin.mode(PullUp);
}

TEST_F(DigitalInTest, setModePullDown)
{
	EXPECT_CALL(mockIOExpander, setPinAsInput(pin_number)).Times(1);
	EXPECT_CALL(mockIOExpander, setModeForPin(pin_number, PullDown)).Times(1);

	leka::io::expanded::DigitalIn<uint16_t> pin {mockIOExpander, pin_number};

	pin.mode(PullDown);
}

TEST_F(DigitalInTest, read)
{
	EXPECT_CALL(mockIOExpander, setPinAsInput(pin_number)).Times(1);
	EXPECT_CALL(mockIOExpander, readPin(pin_number)).Times(1);

	leka::io::expanded::DigitalIn<uint16_t> pin {mockIOExpander, pin_number};

	auto val = pin.read();
}

TEST_F(DigitalInTest, isConnected)
{
	EXPECT_CALL(mockIOExpander, setPinAsInput(pin_number)).Times(1);

	leka::io::expanded::DigitalIn<uint16_t> pin {mockIOExpander, pin_number};

	auto isConnected = pin.is_connected();

	ASSERT_TRUE(isConnected);
}
