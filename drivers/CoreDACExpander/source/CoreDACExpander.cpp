// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACExpander.h"

#include "span"

using namespace leka;

void CoreDACExpanderMCP4728::reset(uint8_t address, uint8_t channel)
{
	auto buffer	 = std::array<uint8_t, 3> {};
	buffer.at(0) = static_cast<uint8_t>(0x40 + ((channel & 0x03) << 1));
	buffer.at(1) = static_cast<uint8_t>(0x00);
	buffer.at(2) = static_cast<uint8_t>(0x00);
	_i2c.write(address, buffer.data(), 3, false);
}

void CoreDACExpanderMCP4728::multiple_write_for_dac_input_registers(uint8_t address, uint8_t channel,
																	uint16_t value_calib)
{
	auto buffer	 = std::array<uint8_t, 3> {};
	buffer.at(0) = static_cast<uint8_t>(0x40 + ((channel & 0x03) << 1));
	buffer.at(1) = static_cast<uint8_t>((value_calib & 0x0F00) >> 8);
	buffer.at(2) = static_cast<uint8_t>((value_calib & 0x00FF) >> 0);
	_i2c.write(address, buffer.data(), 3, false);
}

void CoreDACExpanderMCP4728::sequential_write_for_dac_input_registers_and_eeprom(uint8_t address, uint8_t channel,
																				 uint16_t value_calib)
{
	auto buffer	 = std::array<uint8_t, 3> {};
	buffer.at(0) = static_cast<uint8_t>(0x50 + ((channel & 0x03) << 1));
	buffer.at(1) = static_cast<uint8_t>((value_calib & 0x0F00) >> 8);
	buffer.at(2) = static_cast<uint8_t>((value_calib & 0x00FF) >> 0);
	_i2c.write(address, buffer.data(), 3, false);
}

void CoreDACExpanderMCP4728::single_write_for_dac_input_register_and_eeprom(uint8_t address, uint8_t channel,
																			uint16_t value_calib)
{
	auto buffer	 = std::array<uint8_t, 3> {};
	buffer.at(0) = static_cast<uint8_t>(0x58 + ((channel & 0x03) << 1));
	buffer.at(1) = static_cast<uint8_t>((value_calib & 0x0F00) >> 8);
	buffer.at(2) = static_cast<uint8_t>((value_calib & 0x00FF) >> 0);
	_i2c.write(address, buffer.data(), 3, false);
}

void CoreDACExpanderMCP4728::write_vref(uint8_t address, uint8_t vref)
{
	auto buffer	 = std::array<uint8_t, 1> {};
	buffer.at(0) = static_cast<uint8_t>(0x80 + (0x0F & vref));
	_i2c.write(address, buffer.data(), 1, false);
}

void CoreDACExpanderMCP4728::write_power_down(uint8_t address, uint8_t power_down)
{
	auto buffer	 = std::array<uint8_t, 2> {};
	buffer.at(0) = static_cast<uint8_t>(0xA0 + ((0xF0 & power_down) >> 4));
	buffer.at(1) = static_cast<uint8_t>((0x0F & power_down) << 4);
	_i2c.write(address, buffer.data(), 2, false);
}

void CoreDACExpanderMCP4728::write_gain(uint8_t address, uint8_t gain)
{
	auto buffer	 = std::array<uint8_t, 1> {};
	buffer.at(0) = static_cast<uint8_t>((0xF0 & mcp4728::registers::gain) + (0x0F & gain));
	_i2c.write(address, buffer.data(), 1, false);
}
