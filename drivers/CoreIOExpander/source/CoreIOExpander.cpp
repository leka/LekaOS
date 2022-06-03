// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"

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

	writeRegister(mcp23017::registers::IODIR, _shadow_register.iodir);
	writeRegister(mcp23017::registers::IPOL, _shadow_register.ipol);
	writeRegister(mcp23017::registers::GPINTEN, _shadow_register.gpinten);
	writeRegister(mcp23017::registers::DEFVAL, _shadow_register.defval);
	writeRegister(mcp23017::registers::INTCON, _shadow_register.intcon);
	writeRegister(mcp23017::registers::IOCON, _shadow_register.iocon);
	writeRegister(mcp23017::registers::GPPU, _shadow_register.gppu);
	writeRegister(mcp23017::registers::INTF, _shadow_register.intf);
	writeRegister(mcp23017::registers::INTCAP, _shadow_register.intcap);
	writeRegister(mcp23017::registers::GPIO, _shadow_register.gpio);
	writeRegister(mcp23017::registers::OLAT, _shadow_register.olat);
}

void CoreIOExpanderMCP23017::setPinAsInput(uint16_t pin)
{
	_shadow_register.iodir = readRegister(mcp23017::registers::IODIR);
	_shadow_register.iodir |= pin;
	writeRegister(mcp23017::registers::IODIR, _shadow_register.iodir);
}

auto CoreIOExpanderMCP23017::readInputPin(uint16_t pin) -> int
{
	_shadow_register.iodir = readRegister(mcp23017::registers::IODIR);
	if ((_shadow_register.iodir & pin) != 0) {
		_shadow_register.gpio = readRegister(mcp23017::registers::GPIO);
		auto value			  = _shadow_register.gpio & pin;
		return pin == value ? 1 : 0;
	}
	return 0;
}

void CoreIOExpanderMCP23017::setModeForPin(uint16_t pin, PinMode mode)
{
	_shadow_register.iodir = readRegister(mcp23017::registers::IODIR);
	if ((_shadow_register.iodir & pin) != 0) {
		_shadow_register.gppu = readRegister(mcp23017::registers::GPPU);
		switch (mode) {
			case PinMode::PullUp:
				_shadow_register.gppu |= pin;
				break;
			case PinMode::PullNone:
				_shadow_register.gppu &= ~pin;
				break;
			default:
				break;
		}
		writeRegister(mcp23017::registers::GPPU, _shadow_register.gppu);
	}
}

auto CoreIOExpanderMCP23017::getModeForPin(uint16_t pin) -> PinMode
{
	_shadow_register.iodir = readRegister(mcp23017::registers::IODIR);
	if ((_shadow_register.iodir & pin) != 0) {
		_shadow_register.gppu = readRegister(mcp23017::registers::GPPU);
		auto mode			  = _shadow_register.gppu & pin;

		if (mode != 0) {
			return PinMode::PullUp;
		}
	}
	return PinMode::PullNone;
}

void CoreIOExpanderMCP23017::setPinAsOutput(uint16_t pin)
{
	_shadow_register.iodir = readRegister(mcp23017::registers::IODIR);
	_shadow_register.iodir &= ~pin;
	writeRegister(mcp23017::registers::IODIR, _shadow_register.iodir);
}

auto CoreIOExpanderMCP23017::readOutputPin(uint16_t pin) -> int
{
	_shadow_register.iodir = readRegister(mcp23017::registers::IODIR);
	if ((_shadow_register.iodir & pin) == 0) {
		_shadow_register.gpio = readRegister(mcp23017::registers::GPIO);
		auto value			  = _shadow_register.gpio & pin;
		return pin == value ? 1 : 0;
	}
	return 0;
}

void CoreIOExpanderMCP23017::writeOutputPin(uint16_t pin, int value)
{
	_shadow_register.iodir = readRegister(mcp23017::registers::IODIR);
	if ((_shadow_register.iodir & pin) == 0) {
		_shadow_register.gpio = readRegister(mcp23017::registers::GPIO);
		switch (value) {
			case mcp23017::direction::DIR_INPUT:
				_shadow_register.gpio |= pin;
				break;
			case mcp23017::direction::DIR_OUTPUT:
				_shadow_register.gpio &= ~pin;
				break;
			default:
				break;
		}
		writeRegister(mcp23017::registers::GPIO, _shadow_register.gpio);
	}
}

void CoreIOExpanderMCP23017::writeRegister(uint8_t reg, uint16_t value)
{
	auto data_A	 = static_cast<uint8_t>(0x00FF & value);
	auto data_B	 = static_cast<uint8_t>((0xFF00 & value) >> 8);
	auto command = std::array<uint8_t, 3> {reg, data_A, data_B};

	_i2c.write(_I2C_ADDRESS, command.data(), command.size(), false);
}

auto CoreIOExpanderMCP23017::readRegister(uint8_t reg) -> uint16_t
{
	auto addr	= std::array<uint8_t, 1> {reg};
	auto buffer = std::array<uint8_t, 2> {};

	_i2c.write(_I2C_ADDRESS, addr.data(), addr.size(), true);
	_i2c.read(_I2C_ADDRESS, buffer.data(), buffer.size(), false);

	return static_cast<uint16_t>(buffer[0] + (buffer[1] << 8));
}
