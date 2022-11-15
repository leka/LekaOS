// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"

#include "MemoryUtils.h"
#include "span"

using namespace leka;

CoreIOExpanderMCP23017::CoreIOExpanderMCP23017(interface::I2C &i2c, interface::DigitalOut &mux_reset)
	: _i2c(i2c), _mux_reset(mux_reset)
{
	init();
}

void CoreIOExpanderMCP23017::init()
{
	_mux_reset = 1;

	auto separated_bank_iocon_addr = uint8_t {0x05};
	writeRegister(separated_bank_iocon_addr, 0x0000);

	writeRegister(mcp23017::internal_register::IODIR, _registers.iodir);
	writeRegister(mcp23017::internal_register::IPOL, _registers.ipol);
	writeRegister(mcp23017::internal_register::GPINTEN, _registers.gpinten);
	writeRegister(mcp23017::internal_register::DEFVAL, _registers.defval);
	writeRegister(mcp23017::internal_register::INTCON, _registers.intcon);
	writeRegister(mcp23017::internal_register::IOCON, _registers.iocon);
	writeRegister(mcp23017::internal_register::GPPU, _registers.gppu);
	writeRegister(mcp23017::internal_register::INTF, _registers.intf);
	writeRegister(mcp23017::internal_register::INTCAP, _registers.intcap);
	writeRegister(mcp23017::internal_register::GPIO, _registers.gpio);
	writeRegister(mcp23017::internal_register::OLAT, _registers.olat);
}

void CoreIOExpanderMCP23017::setPinAsInput(uint16_t pin)
{
	_registers.iodir = readRegister(mcp23017::internal_register::IODIR);
	_registers.iodir |= pin;
	writeRegister(mcp23017::internal_register::IODIR, _registers.iodir);
}

void CoreIOExpanderMCP23017::setPinAsOutput(uint16_t pin)
{
	_registers.iodir = readRegister(mcp23017::internal_register::IODIR);
	_registers.iodir &= ~pin;
	writeRegister(mcp23017::internal_register::IODIR, _registers.iodir);
}

void CoreIOExpanderMCP23017::setModeForPin(uint16_t pin, PinMode mode)
{
	_registers.gppu = readRegister(mcp23017::internal_register::GPPU);
	switch (mode) {
		case PinMode::PullUp:
			_registers.gppu |= pin;
			break;
		case PinMode::PullNone:
			_registers.gppu &= ~pin;
			break;
		default:
			break;
	}
	writeRegister(mcp23017::internal_register::GPPU, _registers.gppu);
}

auto CoreIOExpanderMCP23017::getModeForPin(uint16_t pin) -> PinMode
{
	_registers.gppu = readRegister(mcp23017::internal_register::GPPU);

	if ((_registers.gppu & pin) != 0) {
		return PinMode::PullUp;
	}
	return PinMode::PullNone;
}

auto CoreIOExpanderMCP23017::readPin(uint16_t pin) -> int
{
	_registers.gpio = readRegister(mcp23017::internal_register::GPIO);
	auto pin_is_on	= _registers.gpio & pin;
	return pin_is_on;
}

void CoreIOExpanderMCP23017::writePin(uint16_t pin, int value)
{
	_registers.gpio = readRegister(mcp23017::internal_register::GPIO);
	switch (value) {
		case mcp23017::level::high:
			_registers.gpio |= pin;
			break;
		case mcp23017::level::low:
			_registers.gpio &= ~pin;
			break;
		default:
			break;
	}
	writeRegister(mcp23017::internal_register::GPIO, _registers.gpio);
}

void CoreIOExpanderMCP23017::writeRegister(uint8_t reg, uint16_t value)
{
	auto command  = std::array<uint8_t, 3> {};
	command.at(0) = reg;
	command.at(1) = utils::memory::getLowByte(value);
	command.at(2) = utils::memory::getHighByte(value);

	_i2c.write(_I2C_ADDRESS, command.data(), command.size(), false);
}

auto CoreIOExpanderMCP23017::readRegister(uint8_t reg) -> uint16_t
{
	auto addr	= std::array<uint8_t, 1> {reg};
	auto buffer = std::array<uint8_t, 2> {};

	_i2c.write(_I2C_ADDRESS, addr.data(), addr.size(), true);
	_i2c.read(_I2C_ADDRESS, buffer.data(), buffer.size(), false);

	auto value = utils::memory::combineBytes({.high = buffer.at(1), .low = buffer.at(0)});

	return value;
}
