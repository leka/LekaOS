// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQDAC.h"
#include <array>

#include "MemoryUtils.h"

using namespace leka;

void CoreQDACMCP4728::init()
{
	selectVoltageReference(mcp4728::data::voltage_reference::Vdd);
	selectPowerMode(mcp4728::data::power_mode::normal);
	selectGain(mcp4728::data::gain::x1::all);
}

void CoreQDACMCP4728::write(uint8_t channel, uint16_t data, bool b_eep)
{
	if (b_eep) {
		_reg.at(channel).data = _eep.at(channel).data = data;
		singleWrite(channel);
	} else {
		_reg.at(channel).data = data;
		multiWrite();
	}
}

void CoreQDACMCP4728::writeAllChannels(uint16_t data, bool b_eep)
{
	if (b_eep) {
		for (uint8_t ch = 0; ch < 4; ch++) {
			_reg.at(ch).data = _eep.at(ch).data = data;
		}
		sequentialWrite();
	} else {
		for (uint8_t ch = 0; ch < 4; ch++) {
			_reg.at(ch).data = data;
		}
		fastWrite();
	}
}

auto CoreQDACMCP4728::read(uint8_t channel, bool b_eep) -> uint16_t
{
	readInputRegistersAndMemory();
	return b_eep ? _read_eep.at(channel).data : _read_reg.at(channel).data;
}

void CoreQDACMCP4728::fastWrite()
{
	auto command = std::array<uint8_t, 2> {};
	for (uint8_t ch = 0; ch < 4; ch++) {
		command.at(0) = static_cast<uint8_t>(mcp4728::command::fast_write | _reg.at(ch).pd << 4 |
											 (0x0F & utils::memory::getHighByte(_reg.at(ch).data)));
		command.at(1) = utils::memory::getLowByte(_reg.at(ch).data);
		_i2c.write(_address, command.data(), command.size(), true);
	}
}

void CoreQDACMCP4728::multiWrite()
{
	auto command = std::array<uint8_t, 3> {};
	for (uint8_t ch = 0; ch < 4; ch++) {
		command.at(0) = static_cast<uint8_t>(mcp4728::command::multi_write | ((0x03 & ch) << 1));
		command.at(1) = static_cast<uint8_t>(_reg.at(ch).vref << 7 | _reg.at(ch).pd << 5 | _reg.at(ch).gain << 4 |
											 (0x0F & utils::memory::getHighByte(_reg.at(ch).data)));
		command.at(2) = utils::memory::getLowByte(_reg.at(ch).data);
		_i2c.write(_address, command.data(), command.size(), true);
	}
}

void CoreQDACMCP4728::sequentialWrite()
{
	auto command = std::array<uint8_t, 3> {};
	for (uint8_t ch = 0; ch < 4; ch++) {
		command.at(0) = static_cast<uint8_t>(mcp4728::command::sequential_write | ((0x03 & ch) << 1));
		command.at(1) = static_cast<uint8_t>(_eep.at(ch).vref << 7 | _eep.at(ch).pd << 5 | _eep.at(ch).gain << 4 |
											 (0x0F & utils::memory::getHighByte(_eep.at(ch).data)));
		command.at(2) = utils::memory::getLowByte(_eep.at(ch).data);
		_i2c.write(_address, command.data(), command.size(), true);
	}
}

void CoreQDACMCP4728::singleWrite(uint8_t channel)
{
	auto command  = std::array<uint8_t, 3> {};
	command.at(0) = static_cast<uint8_t>(mcp4728::command::single_write | ((0x03 & channel) << 1));
	command.at(1) =
		static_cast<uint8_t>(_eep.at(channel).vref << 7 | _eep.at(channel).pd << 5 | _eep.at(channel).gain << 4 |
							 (0x0F & utils::memory::getHighByte(_eep.at(channel).data)));
	command.at(2) = utils::memory::getLowByte(_eep.at(channel).data);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::selectVoltageReference(uint8_t data)
{
	for (uint8_t ch = 0; ch < 4; ch++) {
		_reg.at(ch).vref = _eep.at(ch).vref = 1 & (data >> (3 - ch));
	}
	auto command = static_cast<uint8_t>(mcp4728::command::select_vref | (0x0F & data));
	_i2c.write(_address, &command, 1, false);
}

void CoreQDACMCP4728::selectPowerMode(uint8_t data)
{
	for (uint8_t ch = 0; ch < 4; ch++) {
		_reg.at(ch).pd = _eep.at(ch).pd = 3 & (data >> (6 - 2 * ch));
	}
	auto command  = std::array<uint8_t, 2> {};
	command.at(0) = static_cast<uint8_t>(mcp4728::command::select_power_down | (0xF0 & data));
	command.at(1) = static_cast<uint8_t>((0x0F & data) << 4);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::selectGain(uint8_t data)
{
	for (uint8_t ch = 0; ch < 4; ch++) {
		_reg.at(ch).gain = _eep.at(ch).gain = 1 & (data >> (3 - ch));
	}
	auto command = static_cast<uint8_t>(mcp4728::command::select_gain | (0x0F & data));
	_i2c.write(_address, &command, 1, false);
}

void CoreQDACMCP4728::readInputRegistersAndMemory()
{
	auto buffer = std::array<uint8_t, mcp4728::command::read::buffer_size> {};

	_i2c.read(_address, buffer.data(), mcp4728::command::read::buffer_size, false);

	for (uint8_t ch = 0; ch < 4; ch++) {
		_read_reg.at(ch).vref = static_cast<uint8_t>((buffer.at(ch * 6 + 1) & 0b10000000) >> 7);
		_read_reg.at(ch).pd	  = static_cast<uint8_t>((buffer.at(ch * 6 + 1) & 0b01100000) >> 5);
		_read_reg.at(ch).gain = static_cast<uint8_t>((buffer.at(ch * 6 + 1) & 0b00010000) >> 4);
		_read_reg.at(ch).data =
			static_cast<uint16_t>(((buffer.at(ch * 6 + 1) & 0b00001111) << 8) | buffer.at(ch * 6 + 2));

		_read_eep.at(ch).vref = static_cast<uint8_t>((buffer.at(ch * 6 + 4) & 0b10000000) >> 7);
		_read_eep.at(ch).pd	  = static_cast<uint8_t>((buffer.at(ch * 6 + 4) & 0b01100000) >> 5);
		_read_eep.at(ch).gain = static_cast<uint8_t>((buffer.at(ch * 6 + 4) & 0b00010000) >> 4);
		_read_eep.at(ch).data =
			static_cast<uint16_t>(((buffer.at(ch * 6 + 4) & 0b00001111) << 8) | buffer.at(ch * 6 + 5));
	}
}
