// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX - License - Identifier : Apache - 2.0

#include "CoreDACTouch.h"
#include <array>

using namespace leka;

void CoreDACTouch::setVoltageReference(uint8_t voltageReference)
{
	uint8_t command = dac_touch::command::setVoltageReference | (0x0f & voltageReference);
	_i2c.write(_address, &command, 1, false);
}

void CoreDACTouch::setPowerMode(uint8_t powerMode)
{
	std::array<uint8_t, 2> command;
	command[0] = static_cast<uint8_t>(dac_touch::command::setPowerMode | ((0xf0 & powerMode) >> 4));
	command[1] = static_cast<uint8_t>((0x0f & powerMode) << 4);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreDACTouch::setGain(uint8_t gain)
{
	uint8_t command = dac_touch::command::setGain | ((0x0f & gain));
	_i2c.write(_address, &command, 1, false);
}

void CoreDACTouch::writeToAllInputRegister(std::array<uint8_t, 2> value)
{
	value[0] = dac_touch::command::writeAllInputRegister | (0x3f & value[0]);

	_i2c.write(_address, value.data(), value.size(), false);
}

void CoreDACTouch::writeToSpecificInputRegister(uint8_t channel, std::array<uint8_t, 2> value)
{
	std::array<uint8_t, 3> command {};
	command[0] = static_cast<uint8_t>(dac_touch::command::writeSpecificInputRegister | ((0x03 & channel) << 1));
	std::copy(value.begin(), value.end(), command.begin() + 1);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreDACTouch::writeToMemoryRegisterUntilChannelD(uint8_t starting_channel, std::array<uint8_t, 2> value)
{
	std::array<uint8_t, 3> command {};
	command[0] =
		static_cast<uint8_t>(dac_touch::command::writeMemoryRegisterUntilChannelD | ((0x03 & starting_channel) << 1));
	std::copy(value.begin(), value.end(), command.begin() + 1);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreDACTouch::writeToSpecificMemoryRegister(uint8_t channel, std::array<uint8_t, 2> value)
{
	std::array<uint8_t, 3> command {};
	command[0] = static_cast<uint8_t>(dac_touch::command::writeSpecificMemoryRegister | ((0x03 & channel) << 1));
	std::copy(value.begin(), value.end(), command.begin() + 1);

	_i2c.write(_address, command.data(), command.size(), false);
}
