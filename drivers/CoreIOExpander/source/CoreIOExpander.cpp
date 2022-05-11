// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreIOExpander.h"

#include "span"

using namespace leka;

CoreIOExpanderMCP23017::CoreIOExpanderMCP23017(interface::I2C &i2c, mbed::DigitalOut &mux_reset)
	: _i2c(i2c), _mux_reset(mux_reset)
{
	init();
	// config_IODIR(0x3F00);
	// config_GPPU(0x3F00);
	// config_GPIO(0x003F);
};

void CoreIOExpanderMCP23017::init()
{
	_mux_reset = 1;

	writeRegister(static_cast<uint8_t>(0x05), static_cast<uint16_t>(0x0000));

	writeRegister(mcp23017::registers::IODIR, static_cast<uint16_t>(0xFFFF));

	for (int reg_addr = 2; reg_addr <= mcp23017::registers::OLAT; reg_addr += 2) {
		writeRegister(reg_addr, static_cast<uint16_t>(0x0000));
	}

	shadow_IODIR = 0xFFFF;
	shadow_GPIO	 = 0;
	shadow_GPPU	 = 0;
	shadow_IPOL	 = 0;
}

void CoreIOExpanderMCP23017::config_IODIR(uint16_t dir_config)
{
	shadow_IODIR = dir_config;
	writeRegister(mcp23017::registers::IODIR, static_cast<uint16_t>(shadow_IODIR));
}

void CoreIOExpanderMCP23017::config_GPPU(uint16_t pullup_config)
{
	shadow_GPPU = pullup_config;
	writeRegister(mcp23017::registers::GPPU, static_cast<uint16_t>(shadow_GPPU));
}

void CoreIOExpanderMCP23017::config_IPOL(uint16_t polarity_config)
{
	shadow_IPOL = polarity_config;
	writeRegister(mcp23017::registers::IPOL, static_cast<uint16_t>(shadow_IPOL));
}

void CoreIOExpanderMCP23017::config_GPIO(uint16_t io_config)
{
	shadow_GPIO = io_config;
	writeRegister(mcp23017::registers::GPIO, static_cast<uint16_t>(shadow_GPIO));
}

void CoreIOExpanderMCP23017::setPinAsInput(uint16_t pin)
{
	shadow_IODIR = readRegister(mcp23017::registers::IODIR);
	shadow_IODIR |= pin;
	writeRegister(mcp23017::registers::IODIR, static_cast<uint16_t>(shadow_IODIR));
}

auto CoreIOExpanderMCP23017::readInputPin(uint16_t pin) -> int
{
	if ((shadow_IODIR & pin) != 0) {
		shadow_GPIO = readRegister(mcp23017::registers::GPIO);
		auto value	= static_cast<int>(shadow_GPIO & (pin));
		return value;
	}
	return 0;
}

void CoreIOExpanderMCP23017::setModeForInputPin(uint16_t pin, PinMode mode)
{
	if ((shadow_IODIR & pin) != 0) {
		auto shadow_GPPU = readRegister(mcp23017::registers::GPPU);
		switch (mode) {
			case PinMode::PullUp:
				shadow_GPPU |= (pin);
				break;
			case PinMode::PullNone:
				shadow_GPPU &= ~(pin);
				break;
			default:
				break;
		}
		writeRegister(mcp23017::registers::GPPU, static_cast<uint16_t>(shadow_GPPU));
	}
}

auto CoreIOExpanderMCP23017::getModeForInputPin(uint16_t pin) -> PinMode
{
	if ((shadow_IODIR & pin) != 0) {
		shadow_GPPU = readRegister(mcp23017::registers::GPPU);
		auto mode	= static_cast<int>(shadow_GPPU & (pin));

		if (mode != 0) {
			return PinMode::PullUp;
		}
	}
	return PinMode::PullNone;
}

void CoreIOExpanderMCP23017::setPinAsOutput(uint16_t pin)
{
	shadow_IODIR = readRegister(mcp23017::registers::IODIR);
	shadow_IODIR &= ~(pin);
	writeRegister(mcp23017::registers::IODIR, static_cast<uint16_t>(shadow_IODIR));
}

auto CoreIOExpanderMCP23017::readOutputPin(uint16_t pin) -> int
{
	if ((shadow_IODIR & pin) == 0) {
		shadow_GPIO = readRegister(mcp23017::registers::GPIO);
		auto value	= static_cast<int>(shadow_GPIO & (pin));
		return value;
	}
	return 0;
}

void CoreIOExpanderMCP23017::writeOutputPin(uint16_t pin, int value)
{
	if ((shadow_IODIR & pin) == 0) {
		shadow_GPIO = readRegister(mcp23017::registers::GPIO);
		switch (value) {
			case 0x01:
				shadow_GPIO |= (pin);
				break;
			case 0x00:
				shadow_GPIO &= ~(pin);
				break;
			default:
				break;
		}
		writeRegister(mcp23017::registers::GPIO, static_cast<uint16_t>(shadow_GPIO));
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

	// TODO(mourad): Add Mutex Lock and Unlock ?

	_i2c.write(_I2C_ADDRESS, addr.data(), addr.size(), true);
	_i2c.read(_I2C_ADDRESS, buffer.data(), buffer.size(), false);

	return static_cast<uint16_t>(buffer[0] + (buffer[1] << 8));
}
