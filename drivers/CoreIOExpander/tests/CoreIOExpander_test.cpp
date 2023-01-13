// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"

#include "MemoryUtils.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mocks/leka/CoreI2C.h"
#include "mocks/mbed/DigitalOut.h"
#include "span"

using namespace leka;
using ::testing::_;
using ::testing::Args;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::InSequence;
using ::testing::Pointee;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;

class CoreIOExpanderTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	const uint8_t i2c_address {0x4E};
	mock::CoreI2C mocki2c;

	mbed::mock::DigitalOut expander_reset {};

	const uint16_t pin_number {0x0100};

	void MOCK_FUNCTION_expectedCallsReadRegister(uint8_t reg, std::array<uint8_t, 2> &expected_buffer)
	{
		{
			InSequence seq;

			auto expected_reg = ElementsAre(reg);

			EXPECT_CALL(mocki2c, write(i2c_address, _, 1, true)).With(Args<1, 2>(expected_reg)).Times(1);
			EXPECT_CALL(mocki2c, read(i2c_address, _, 2, false))
				.WillOnce(
					DoAll(SetArrayArgument<1>(std::begin(expected_buffer), std::end(expected_buffer)), Return(0)));
		}
	}

	void MOCK_FUNCTION_expectedCallsWriteRegister(uint8_t reg, std::array<uint8_t, 2> &expected_buffer)
	{
		{
			InSequence seq;

			auto expected_command = ElementsAre(reg, expected_buffer.at(0), expected_buffer.at(1));
			EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).With(Args<1, 2>(expected_command)).Times(1);
		}
	}

	void MOCK_FUNCTION_expectedCallsInitAllRegisters()
	{
		{
			InSequence seq;
			EXPECT_CALL(expander_reset, write);
			EXPECT_CALL(mocki2c, write(i2c_address, _, 3, false)).Times(1);
			std::array<uint8_t, 2> IODIR_expected_buffer {0xFF, 0xFF};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::IODIR, IODIR_expected_buffer);
			std::array<uint8_t, 2> IPOL_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::IPOL, IPOL_expected_buffer);
			std::array<uint8_t, 2> GPINTEN_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPINTEN, GPINTEN_expected_buffer);
			std::array<uint8_t, 2> DEFVAL_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::DEFVAL, DEFVAL_expected_buffer);
			std::array<uint8_t, 2> INTCON_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::INTCON, INTCON_expected_buffer);
			std::array<uint8_t, 2> IOCON_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::IOCON, IOCON_expected_buffer);
			std::array<uint8_t, 2> GPPU_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);
			std::array<uint8_t, 2> INTF_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::INTF, INTF_expected_buffer);
			std::array<uint8_t, 2> INTCAP_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::INTCAP, INTCAP_expected_buffer);
			std::array<uint8_t, 2> GPIO_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);
			std::array<uint8_t, 2> OLAT_expected_buffer {0x00, 0x00};
			MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::OLAT, OLAT_expected_buffer);
		}
	}
};

TEST_F(CoreIOExpanderTest, initializationDefault)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};
	EXPECT_NE(&expander, nullptr);
}

TEST_F(CoreIOExpanderTest, setPinAsInput)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::IODIR, IODIR_expected_buffer);

	IODIR_expected_buffer.at(0) |= (utils::memory::getLowByte(pin_number));
	IODIR_expected_buffer.at(1) |= (utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::IODIR, IODIR_expected_buffer);

	expander.setPinAsInput(pin_number);
}

TEST_F(CoreIOExpanderTest, setPinAsOutput)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> IODIR_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::IODIR, IODIR_expected_buffer);

	IODIR_expected_buffer.at(0) &= ~(utils::memory::getLowByte(pin_number));
	IODIR_expected_buffer.at(1) &= ~(utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::IODIR, IODIR_expected_buffer);

	expander.setPinAsOutput(pin_number);
}

TEST_F(CoreIOExpanderTest, readPinHigh)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	GPIO_expected_buffer.at(0) |= (utils::memory::getLowByte(pin_number));
	GPIO_expected_buffer.at(1) |= (utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);

	auto output_value = expander.readPin(pin_number);
	EXPECT_EQ(pin_number, output_value);
}

TEST_F(CoreIOExpanderTest, readPinLow)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	GPIO_expected_buffer.at(0) &= ~(utils::memory::getLowByte(pin_number));
	GPIO_expected_buffer.at(1) &= ~(utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);

	auto output_value = expander.readPin(pin_number);
	EXPECT_EQ(0, output_value);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullUp)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);

	GPPU_expected_buffer.at(0) |= (utils::memory::getLowByte(pin_number));
	GPPU_expected_buffer.at(1) |= (utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);

	expander.setModeForPin(pin_number, PinMode::PullUp);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullNone)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);

	GPPU_expected_buffer.at(0) &= ~(utils::memory::getLowByte(pin_number));
	GPPU_expected_buffer.at(1) &= ~(utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);

	expander.setModeForPin(pin_number, PinMode::PullNone);
}

TEST_F(CoreIOExpanderTest, setModeForPinPullDown)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);

	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);

	expander.setModeForPin(pin_number, PinMode::PullDown);
}

TEST_F(CoreIOExpanderTest, getModeForPinPullUp)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	GPPU_expected_buffer.at(0) |= (utils::memory::getLowByte(pin_number));
	GPPU_expected_buffer.at(1) |= (utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);

	auto mode = expander.getModeForPin(pin_number);
	EXPECT_EQ(PinMode::PullUp, mode);
}

TEST_F(CoreIOExpanderTest, getModeForPinPullNone)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPPU_expected_buffer {};
	GPPU_expected_buffer.at(0) &= ~(utils::memory::getLowByte(pin_number));
	GPPU_expected_buffer.at(1) &= ~(utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPPU, GPPU_expected_buffer);

	auto mode = expander.getModeForPin(pin_number);
	EXPECT_EQ(PinMode::PullNone, mode);
}

TEST_F(CoreIOExpanderTest, writeOutputPinHigh)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);

	GPIO_expected_buffer.at(0) |= (utils::memory::getLowByte(pin_number));
	GPIO_expected_buffer.at(1) |= (utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);

	expander.writePin(pin_number, mcp23017::level::high);
}

TEST_F(CoreIOExpanderTest, writeOutputPinLow)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);

	GPIO_expected_buffer.at(0) &= ~(utils::memory::getLowByte(pin_number));
	GPIO_expected_buffer.at(1) &= ~(utils::memory::getHighByte(pin_number));
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);

	expander.writePin(pin_number, mcp23017::level::low);
}

TEST_F(CoreIOExpanderTest, writeOutputPinBis)
{
	MOCK_FUNCTION_expectedCallsInitAllRegisters();
	auto expander = CoreIOExpanderMCP23017 {mocki2c, expander_reset};

	std::array<uint8_t, 2> GPIO_expected_buffer {};
	MOCK_FUNCTION_expectedCallsReadRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);
	MOCK_FUNCTION_expectedCallsWriteRegister(mcp23017::internal_register::GPIO, GPIO_expected_buffer);

	expander.writePin(pin_number, 0x02);
}
