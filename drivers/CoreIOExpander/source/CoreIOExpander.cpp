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
void MCP23017::setRegisterMappingSeparated(){
	uint16_t IOCON_config = readRegister(mcp23017::registers::IOCON);
	writeRegister(mcp23017::registers::IOCON, IOCON_config | 0x8080);

}

void MCP23017::setRegisterMappingUnified(){
	uint16_t IOCON_config = readRegister(mcp23017::registers::IOCON);
	writeRegister(mcp23017::registers::IOCON, IOCON_config & 0x7F7F);
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
	buffer[1] = getFirstUint8ValueFromUint16Value(value);
	buffer[2] = getSecondUint8ValueFromUint16Value(value);

	_i2c.write(_I2C_ADDRESS, buffer.data(), buffer.size());
}

auto MCP23017::getFirstUint8ValueFromUint16Value(uint16_t value) -> uint8_t{
	constexpr uint16_t mask = 0x00FF;
	return (mask & value);
}


auto MCP23017::getSecondUint8ValueFromUint16Value(uint16_t value) -> uint8_t{
	constexpr uint16_t mask = 0xFF00;
	return (mask & value) >> 8;
}

auto MCP23017::readRegister(uint8_t reg) -> uint16_t
{
	auto buffer = std::array<uint8_t, 2> {reg, 0};

	mutex.lock();

	_i2c.write(_I2C_ADDRESS, buffer.data(), 1);
	_i2c.read(_I2C_ADDRESS, buffer.data(), 2);

	mutex.unlock();

	return getReadValueAsUint16(buffer);
}

auto MCP23017::getReadValueAsUint16(std::array<uint8_t, 2> &buffer) -> uint16_t{
	return static_cast<uint16_t>(buffer[0] + (buffer[1] << 8));
}



void MCP23017::setInputPins(uint16_t pins)
{
	auto value = readRegister(mcp23017::registers::IODIR);

	updateInputPins(value, pins);
	writeRegister(mcp23017::registers::IODIR, value);
}

void MCP23017::updateInputPins(uint16_t &value, uint16_t pins){
	value |= pins;
}

void MCP23017::setOutputPins(uint16_t pins)
{
	auto value = readRegister(mcp23017::registers::IODIR);

	updateOutputPins(value, pins);
	writeRegister(mcp23017::registers::IODIR, value & ~pins);
}

void MCP23017::updateOutputPins(uint16_t &value, uint16_t pins){
	value &= ~pins;
}

void MCP23017::writeOutputs(uint16_t values)
{
	uint16_t outputs_values = getOutputsValue(values);
	writeRegister(mcp23017::registers::GPIO, outputs_values);
}

auto MCP23017::getOutputsValue(uint16_t value) -> uint16_t{
	return (0xFF00 & value << 8) + (0x00FF & value >> 8);
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
	uint16_t source_interrupt = readRegister(mcp23017::registers::INTCON);
	updateSourceOfInterrupt(source_interrupt, pins);
	writeRegister(mcp23017::registers::INTCON, source_interrupt);

	uint16_t interrupt_enable = readRegister(mcp23017::registers::GPINTEN);
	updateInterruptEnable(interrupt_enable, pins);
	writeRegister(mcp23017::registers::GPINTEN, interrupt_enable);
}

void MCP23017::updateSourceOfInterrupt(uint16_t &value, uint16_t pins){
	value &= ~pins;
}

void MCP23017::updateInterruptEnable(uint16_t &value, uint16_t pins){
	value |= pins;
}

void MCP23017::disableInterrupts(uint16_t pins)
{
	uint16_t interrup_value = readRegister(mcp23017::registers::GPINTEN);
	updateSourceOfInterrupt(interrup_value, pins);

	writeRegister(mcp23017::registers::GPINTEN, interrup_value);
}

void MCP23017::acknowledgeInterrupt(uint16_t &pin, uint16_t &values)
{
	pin	   = readRegister(mcp23017::registers::INTF);
	values = readRegister(mcp23017::registers::INTCAP);
}
