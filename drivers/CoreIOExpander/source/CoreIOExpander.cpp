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

void MCP23017::ExpandedIO::internalMode(PinMode mode)
{
	if (mode != PullDown) {	  // ? MCP23017 may not support PullDown mode

		_parent.mutex.lock();
		uint16_t pullups = _parent.getPullups();

		if (mode == PullNone) {
			pullups &= ~_pin;
		} else if (mode == PullUp) {
			pullups |= _pin;
		}

		_parent.setPullups(pullups);
		_parent.mutex.unlock();
	}
}

void MCP23017::ExpandedIO::internalInput()
{
	_parent.setInputPins(_pin);
}

auto MCP23017::asInput(uint16_t pin) -> MCP23017::ExpandedInput
{
	return ExpandedInput(*this, pin);
}

void MCP23017::init(uint16_t input_pins)
{
	reset();
	setInputPins(input_pins);
	setPullups(input_pins);
	writeOutputs(input_pins);
}

// TODO (@benjamin) - make two functions: setRegisterMappingSeparated() and setRegisterMappingUnified()
void MCP23017::setRegisterMapping(bool separated)
{
	uint16_t IOCON_config = readRegister(mcp23017::registers::IOCON);
	if (separated) {
		// TODO (@benjamin) - create small private function for this and use constexpr variable for mask
		writeRegister(mcp23017::registers::IOCON, IOCON_config | 0x8080);
	} else {
		// TODO (@benjamin) - create small private function for this and use constexpr variable for mask
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
	// TODO (@benjamin) - create small private function for this and use constexpr variable for mask
	buffer[1] = (0x00FF & value);
	// TODO (@benjamin) - create small private function for this and use constexpr variable for mask
	buffer[2] = (0xFF00 & value) >> 8;

	_i2c.write(_I2C_ADDRESS, buffer.data(), buffer.size());
}

auto MCP23017::readRegister(uint8_t reg) -> uint16_t
{
	auto buffer = std::array<uint8_t, 2> {reg, 0};

	mutex.lock();

	_i2c.write(_I2C_ADDRESS, buffer.data(), 1);
	_i2c.read(_I2C_ADDRESS, buffer.data(), 2);

	mutex.unlock();

	// TODO (@benjamin) - create small private function for this as it's unclear what it does
	auto val = static_cast<uint16_t>(buffer[0] + (buffer[1] << 8));

	return val;
}

void MCP23017::setInputPins(uint16_t pins)
{
	auto value = readRegister(mcp23017::registers::IODIR);

	// TODO (@benjamin) - explain what happens here with small function
	writeRegister(mcp23017::registers::IODIR, value | pins);
}

void MCP23017::setOutputPins(uint16_t pins)
{
	auto value = readRegister(mcp23017::registers::IODIR);

	// TODO (@benjamin) - explain what happens here with small function
	writeRegister(mcp23017::registers::IODIR, value & ~pins);
}

void MCP23017::writeOutputs(uint16_t values)
{
	// TODO (@benjamin) - explain what happens here with small function
	// TODO (@benjamin) - olat?
	uint16_t olat_values = (0xFF00 & values << 8) + (0x00FF & values >> 8);	  // results from testing the MCP23017

	writeRegister(mcp23017::registers::GPIO, olat_values);
}

auto MCP23017::readOutputs() -> uint16_t
{
	return readRegister(mcp23017::registers::OLAT);
}

auto MCP23017::readInputs() -> uint16_t
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
	// TODO (@benjamin) - explain what happens here with small function
	value &= ~pins;
	writeRegister(mcp23017::registers::INTCON, value);

	value = readRegister(mcp23017::registers::GPINTEN);
	// TODO (@benjamin) - explain what happens here with small function
	value |= pins;
	writeRegister(mcp23017::registers::GPINTEN, value);
}

void MCP23017::disableInterrupts(uint16_t pins)
{
	uint16_t value = readRegister(mcp23017::registers::GPINTEN);
	// TODO (@benjamin) - explain what happens here with small function
	value &= ~pins;
	writeRegister(mcp23017::registers::GPINTEN, value);
}

void MCP23017::acknowledgeInterrupt(uint16_t &pin, uint16_t &values)
{
	pin	   = readRegister(mcp23017::registers::INTF);
	values = readRegister(mcp23017::registers::INTCAP);
}
