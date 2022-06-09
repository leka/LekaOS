// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTouch.h"
#include <array>

using namespace leka;

void CoreDACTouchMCP4728::writeToAllInputRegister(std::array<uint8_t, 2> value)
{
	auto command  = std::array<uint8_t, 2> {};
	command.at(0) = static_cast<uint8_t>((0xC0 & mcp4728::command::writeAllInputRegister) | (0x3F & value.at(0)));
	command.at(1) = value.at(1);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreDACTouchMCP4728::writeToSpecificInputRegister(uint8_t channel, std::array<uint8_t, 2> value)
{
	auto command = std::array<uint8_t, 3> {};
	command.at(0) =
		static_cast<uint8_t>((0xF8 & mcp4728::command::writeSpecificInputRegister) | (0x07 & ((0x03 & channel) << 1)));
	std::copy(value.begin(), value.end(), command.begin() + 1);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreDACTouchMCP4728::writeToMemoryRegisterUntilChannelD(uint8_t starting_channel, std::array<uint8_t, 2> value)
{
	auto command  = std::array<uint8_t, 3> {};
	command.at(0) = static_cast<uint8_t>((0xF8 & mcp4728::command::writeMemoryRegisterUntilChannelD) |
										 (0x07 & ((0x03 & starting_channel) << 1)));
	std::copy(value.begin(), value.end(), command.begin() + 1);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreDACTouchMCP4728::writeToSpecificMemoryRegister(uint8_t channel, std::array<uint8_t, 2> value)
{
	auto command = std::array<uint8_t, 3> {};
	command.at(0) =
		static_cast<uint8_t>((0xF8 & mcp4728::command::writeSpecificMemoryRegister) | (0x07 & ((0x03 & channel) << 1)));
	std::copy(value.begin(), value.end(), command.begin() + 1);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreDACTouchMCP4728::writeVoltageReference(uint8_t voltageReference)
{
	uint8_t command = (0xF8 & mcp4728::command::writeVoltageReference) | (0x0F & voltageReference);
	_i2c.write(_address, &command, 1, false);
}

void CoreDACTouchMCP4728::writePowerMode(uint8_t powerMode)
{
	auto command  = std::array<uint8_t, 2> {};
	command.at(0) = static_cast<uint8_t>((0xF8 & mcp4728::command::writePowerMode) | ((0xF0 & powerMode) >> 4));
	command.at(1) = static_cast<uint8_t>((0x0F & powerMode) << 4);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreDACTouchMCP4728::writeGain(uint8_t gain)
{
	uint8_t command = mcp4728::command::writeGain | ((0x0F & gain));
	_i2c.write(_address, &command, 1, false);
}

void CoreDACTouchMCP4728::readMemory(std::array<uint8_t, 24> &value)
{
	_i2c.read(_address, value.data(), value.size(), false);
}
