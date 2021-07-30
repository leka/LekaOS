// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"
#include <array>
#include <cstdint>

using namespace leka;

auto MCP23017::ExpandedIO::internalRead() -> int
{
	return (_parent.readInputs() & _pin);
}

void MCP23017::ExpandedIO::internalMode(PinMode pull)
{
	if (pull != PullDown) {	  // MCP23017 may not support PullDown mode

		_parent.mutex.lock();
		uint16_t pullups = _parent.getPullups();

		if (pull == PullNone) {
			pullups &= ~_pin;
		} else if (pull == PullUp) {
			pullups |= _pin;
		}

		_parent.setPullups(pullups);
		_parent.mutex.unlock();
	}
}

// void MCP23017::ExpandedIO::internalWrite(int value)
// {
// 	uint16_t outputs = _parent.readOutputs();
// 	if (value) {
// 		_parent.writeOutputs((outputs | _pin));
// 	} else {
// 		_parent.writeOutputs((outputs & (~_pin)));
// 	}
// }

// void MCP23017::ExpandedIO::internalOutput()
// {
// 	_parent.setOutputPins(_pin);
// }

void MCP23017::ExpandedIO::internalInput()
{
	_parent.setInputPins(_pin);
}

MCP23017::ExpandedInput MCP23017::asInput(MCP23017::Pin pin)
{
	return ExpandedInput(*this, pin);
}

void MCP23017::init()
{
	reset();
}

void MCP23017::setRegisterMapping(bool separated)
{
	uint16_t IOCON_config = readRegister(mcp23017::registers::IOCON);
	if (separated) {
		writeRegister(mcp23017::registers::IOCON, IOCON_config | 0x8080);
	} else {
		writeRegister(mcp23017::registers::IOCON, IOCON_config & 0x7F7F);
	}
}

void MCP23017::reset()
{
	for (uint8_t reg_addr = 0; reg_addr <= mcp23017::registers::OLAT; reg_addr += 2) {
		writeRegister(reg_addr, 0x0000);
	}
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

	return static_cast<uint16_t>(buffer[0] + (buffer[1] << 8));
}

void MCP23017::setInputPins(uint16_t pins)
{
	auto value = readRegister(mcp23017::registers::IODIR);

	writeRegister(mcp23017::registers::IODIR, value | pins);
}

void MCP23017::setOutputPins(uint16_t pins)
{
	auto value = readRegister(mcp23017::registers::IODIR);

	writeRegister(mcp23017::registers::IODIR, value & ~pins);
}

void MCP23017::writeOutputs(uint16_t values)
{
	writeRegister(mcp23017::registers::GPIO, values);
}

auto MCP23017::readOutputs() -> uint16_t
{
	return readRegister(mcp23017::registers::OLAT);
}

uint16_t MCP23017::readInputs()
{
	return readRegister(mcp23017::registers::GPIO);
}

void MCP23017::setInputPolarity(uint16_t values)
{
	writeRegister(mcp23017::registers::IPOL, values);
}

auto MCP23017::getInputPolarity() -> uint16_t
{
	return readRegister(mcp23017::registers::IPOL);
}

void MCP23017::setPullups(uint16_t values)
{
	writeRegister(mcp23017::registers::GPPU, values);
}

auto MCP23017::getPullups() -> uint16_t
{
	return readRegister(mcp23017::registers::GPPU);
}

void MCP23017::interruptOnChanges(uint16_t pins)
{
	uint16_t value = readRegister(mcp23017::registers::INTCON);
	value &= ~pins;
	writeRegister(mcp23017::registers::INTCON, value);

	value = readRegister(mcp23017::registers::GPINTEN);
	value |= pins;
	writeRegister(mcp23017::registers::GPINTEN, value);
}

void MCP23017::disableInterrupts(uint16_t pins)
{
	uint16_t value = readRegister(mcp23017::registers::GPINTEN);
	value &= ~pins;
	writeRegister(mcp23017::registers::GPINTEN, value);
}

void MCP23017::acknowledgeInterrupt(uint16_t &pin, uint16_t &values)
{
	pin	   = readRegister(mcp23017::registers::INTF);
	values = readRegister(mcp23017::registers::INTCAP);
}
