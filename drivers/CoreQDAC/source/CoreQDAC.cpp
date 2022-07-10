// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQDAC.h"
#include <array>

#include "MemoryUtils.h"

using namespace leka;

void CoreQDACMCP4728::init()
{
	writeVoltageReference(mcp4728::data::voltage_reference::Vdd);
	writePowerMode(mcp4728::data::power_mode::normal);
	writeGain(mcp4728::data::gain::x1::all);
}

void CoreQDACMCP4728::write(uint8_t channel, std::array<uint8_t, 2> data)
{
	writeToSingleInputRegisterAndMemory(channel, data);
}

void CoreQDACMCP4728::read(std::span<uint8_t> buffer)
{
	readInputRegistersAndMemory(buffer);
}

void CoreQDACMCP4728::writeToMultipleInputRegisters(uint8_t channel, std::array<uint8_t, 2> data)
{
	auto command = std::array<uint8_t, 3> {};
	command.at(0) =
		static_cast<uint8_t>((mcp4728::command::write::input_registers::mask::field &
							  (mcp4728::command::write::input_registers::multi_default)) |
							 (mcp4728::command::write::input_registers::mask::channel & ((0x03 & channel) << 1)));
	command.at(1) = data.at(0);
	command.at(2) = data.at(1);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::writeToInputRegistersAndMemoryUntilChannelD(uint8_t channel, std::array<uint8_t, 2> data)
{
	auto command = std::array<uint8_t, 3> {};
	command.at(0) =
		static_cast<uint8_t>((mcp4728::command::write::input_registers::mask::field &
							  (mcp4728::command::write::input_registers::sequential_until_channel_D)) |
							 (mcp4728::command::write::input_registers::mask::channel & ((0x03 & channel) << 1)));
	command.at(1) = data.at(0);
	command.at(2) = data.at(1);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::writeToSingleInputRegisterAndMemory(uint8_t channel, std::array<uint8_t, 2> data)
{
	auto command = std::array<uint8_t, 3> {};
	command.at(0) =
		static_cast<uint8_t>((mcp4728::command::write::input_registers::mask::field &
							  (mcp4728::command::write::input_registers::single_with_eeprom_memory)) |
							 (mcp4728::command::write::input_registers::mask::channel & ((0x03 & channel) << 1)));
	command.at(1) = data.at(0);
	command.at(2) = data.at(1);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::writeVoltageReference(uint8_t voltageReference)
{
	auto mask	 = mcp4728::command::write::select_bits::mask::field;
	auto command = static_cast<uint8_t>((mask & mcp4728::command::write::select_bits::voltage_reference) |
										(~mask & voltageReference));
	_i2c.write(_address, &command, 1, false);
}

void CoreQDACMCP4728::writePowerMode(uint8_t powerMode)
{
	auto mask	  = mcp4728::command::write::select_bits::mask::field;
	auto command  = std::array<uint8_t, 2> {};
	command.at(0) = static_cast<uint8_t>((mask & mcp4728::command::write::select_bits::power_mode) |
										 (~mask & (0xF0 & powerMode) >> 4));
	command.at(1) = static_cast<uint8_t>((0x0F & powerMode) << 4);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::writeGain(uint8_t gain)
{
	auto mask	 = mcp4728::command::write::select_bits::mask::field;
	auto command = static_cast<uint8_t>((mask & mcp4728::command::write::select_bits::gain) | (~mask & gain));
	_i2c.write(_address, &command, 1, false);
}

void CoreQDACMCP4728::readInputRegistersAndMemory(std::span<uint8_t> buffer)
{
	_i2c.read(_address, buffer.data(), buffer.size(), false);
}
