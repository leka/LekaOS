// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "MCP23017.h"
#include <array>
#include <cstdint>

using namespace leka;

void MCP23017::init()
{
	reset();
}

void MCP23017::reset()
{
	//
	// First make sure that the device is in BANK=0 mode
	//
	writeRegister(0x05, (unsigned char)0x00);
	//
	// set direction registers to inputs
	//
	writeRegister(mcp23017::registers::IODIR, (unsigned short)0xFFFF);
	//
	// set all other registers to zero (last of 10 registers is OLAT)
	//
	for (int reg_addr = 2; reg_addr <= mcp23017::registers::OLAT; reg_addr += 2) {
		writeRegister(reg_addr, (unsigned short)0x0000);
	}
	//
	// Set the shadow registers to power-on state
	//
	shadow_IODIR   = 0xFFFF;
	shadow_GPIO	   = 0;
	shadow_GPPU	   = 0;
	shadow_IPOL	   = 0;
	shadow_GPINTEN = 0;
}

void MCP23017::writeRegister(uint8_t reg, uint16_t value)
{
	std::array<uint8_t, 3> buffer {};

	buffer[0] = reg;
	buffer[1] = (0x00FF & value);
	buffer[2] = (0xFF00 & value) >> 8;

	_i2c.write(_I2C_ADDRESS, buffer.data(), buffer.size());
}

auto MCP23017::readRegister(uint8_t reg) -> uint16_t
{
	std::array<uint8_t, 2> buffer {reg, 0};

	mutex.lock();

	_i2c.write(_I2C_ADDRESS, buffer.data(), 1);
	_i2c.read(_I2C_ADDRESS, buffer.data(), 2);

	mutex.unlock();

	return (buffer[0] + (buffer[1] << 8));
}

void MCP23017::setInputPin(uint8_t pins)
{
	auto value = readRegister(mcp23017::registers::IODIR);

	writeRegister(mcp23017::registers::IODIR, value | pins);
}

void MCP23017::setOutputPin(uint8_t pins)
{
	auto value = readRegister(mcp23017::registers::IODIR);

	writeRegister(mcp23017::registers::IODIR, value & ~pins);
}

uint16_t MCP23017::readInputs()
{
	return readRegister(mcp23017::registers::GPIO);
}
