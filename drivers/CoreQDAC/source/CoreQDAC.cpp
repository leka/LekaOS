// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQDAC.h"
#include <array>

#include "MemoryUtils.h"

using namespace leka;

void CoreQDACMCP4728::init()
{
	setVoltageReference(mcp4728::data::voltage_reference::Vdd);
	setPowerDown(mcp4728::data::power_down::normal);
	setGain(mcp4728::data::gain::x1::all);
}

void CoreQDACMCP4728::write(Channel channel, uint16_t data, bool eeprom)
{
	auto ch = static_cast<uint8_t>(channel);
	if (eeprom) {
		_tx_registers.at(ch).data = data;
		_tx_eeprom.at(ch).data	  = data;
		singleWrite(ch);
	} else {
		_tx_registers.at(ch).data = data;
		multiWrite(ch);
	}
}

void CoreQDACMCP4728::writeAllChannels(uint16_t data, bool eeprom)
{
	if (eeprom) {
		for (uint8_t ch = mcp4728::channel::A; ch <= mcp4728::channel::D; ch++) {
			_tx_registers.at(ch).data = data;
			_tx_eeprom.at(ch).data	  = data;
		}
		sequentialWrite(mcp4728::channel::A);
	} else {
		for (uint8_t ch = mcp4728::channel::A; ch <= mcp4728::channel::D; ch++) {
			_tx_registers.at(ch).data = data;
		}
		fastWrite();
	}
}

auto CoreQDACMCP4728::read(Channel channel, bool eeprom) -> uint16_t
{
	auto ch = static_cast<uint8_t>(channel);
	readInputRegistersAndMemory();
	return eeprom ? _rx_eeprom.at(ch).data : _rx_registers.at(ch).data;
}

void CoreQDACMCP4728::fastWrite()
{
	auto command = std::array<uint8_t, 8> {};
	for (uint8_t ch = mcp4728::channel::A; ch <= mcp4728::channel::D; ch++) {
		command.at(ch * 2)	   = static_cast<uint8_t>(mcp4728::command::fast_write | _tx_registers.at(ch).pd << 4 |
												  (0x0F & utils::memory::getHighByte(_tx_registers.at(ch).data)));
		command.at(ch * 2 + 1) = utils::memory::getLowByte(_tx_registers.at(ch).data);
	}
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::multiWrite(uint8_t channel)
{
	auto command  = std::array<uint8_t, 3> {};
	command.at(0) = static_cast<uint8_t>(mcp4728::command::multi_write | ((0x03 & channel) << 1));
	command.at(1) = static_cast<uint8_t>(_tx_registers.at(channel).vref << 7 | _tx_registers.at(channel).pd << 5 |
										 _tx_registers.at(channel).gain << 4 |
										 (0x0F & utils::memory::getHighByte(_tx_registers.at(channel).data)));
	command.at(2) = utils::memory::getLowByte(_tx_registers.at(channel).data);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::sequentialWrite(uint8_t starting_channel)
{
	auto command  = std::array<uint8_t, 9> {};
	command.at(0) = static_cast<uint8_t>(mcp4728::command::sequential_write | ((0x03 & starting_channel) << 1));
	for (uint8_t ch = starting_channel; ch <= mcp4728::channel::D; ch++) {
		command.at((ch - starting_channel) * 2 + 1) =
			static_cast<uint8_t>(_tx_eeprom.at(ch).vref << 7 | _tx_eeprom.at(ch).pd << 5 | _tx_eeprom.at(ch).gain << 4 |
								 (0x0F & utils::memory::getHighByte(_tx_eeprom.at(ch).data)));
		command.at((ch - starting_channel) * 2 + 2) = utils::memory::getLowByte(_tx_eeprom.at(ch).data);
	}
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::singleWrite(uint8_t channel)
{
	auto command  = std::array<uint8_t, 3> {};
	command.at(0) = static_cast<uint8_t>(mcp4728::command::single_write | ((0x03 & channel) << 1));
	command.at(1) = static_cast<uint8_t>(_tx_eeprom.at(channel).vref << 7 | _tx_eeprom.at(channel).pd << 5 |
										 _tx_eeprom.at(channel).gain << 4 |
										 (0x0F & utils::memory::getHighByte(_tx_eeprom.at(channel).data)));
	command.at(2) = utils::memory::getLowByte(_tx_eeprom.at(channel).data);
	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::setVoltageReference(uint8_t data)
{
	for (uint8_t ch = mcp4728::channel::A; ch <= mcp4728::channel::D; ch++) {
		_tx_registers.at(ch).vref = 1 & (data >> (3 - ch));
		_tx_eeprom.at(ch).vref	  = 1 & (data >> (3 - ch));
	}
	auto command = static_cast<uint8_t>(mcp4728::command::set_vref | (0x0F & data));
	_i2c.write(_address, &command, 1, false);
}

void CoreQDACMCP4728::setPowerDown(uint8_t data)
{
	for (uint8_t ch = mcp4728::channel::A; ch <= mcp4728::channel::D; ch++) {
		_tx_registers.at(ch).pd = 3 & (data >> (6 - 2 * ch));
		_tx_eeprom.at(ch).pd	= 3 & (data >> (6 - 2 * ch));
	}
	auto command  = std::array<uint8_t, 2> {};
	command.at(0) = static_cast<uint8_t>(mcp4728::command::set_power_down | (0xF0 & data) >> 4);
	command.at(1) = static_cast<uint8_t>((0x0F & data) << 4);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::setGain(uint8_t data)
{
	for (uint8_t ch = mcp4728::channel::A; ch <= mcp4728::channel::D; ch++) {
		_tx_registers.at(ch).gain = 1 & (data >> (3 - ch));
		_tx_eeprom.at(ch).gain	  = 1 & (data >> (3 - ch));
	}
	auto command = static_cast<uint8_t>(mcp4728::command::set_gain | (0x0F & data));
	_i2c.write(_address, &command, 1, false);
}

void CoreQDACMCP4728::readInputRegistersAndMemory()
{
	auto buffer = std::array<uint8_t, mcp4728::command::read::buffer_size> {};

	_i2c.read(_address, buffer.data(), buffer.size(), false);

	for (uint8_t ch = mcp4728::channel::A; ch <= mcp4728::channel::D; ch++) {
		_rx_registers.at(ch).vref = static_cast<uint8_t>((buffer.at(ch * 6 + 1) & 0b10000000) >> 7);
		_rx_registers.at(ch).pd	  = static_cast<uint8_t>((buffer.at(ch * 6 + 1) & 0b01100000) >> 5);
		_rx_registers.at(ch).gain = static_cast<uint8_t>((buffer.at(ch * 6 + 1) & 0b00010000) >> 4);
		_rx_registers.at(ch).data =
			static_cast<uint16_t>(((buffer.at(ch * 6 + 1) & 0b00001111) << 8) | buffer.at(ch * 6 + 2));

		_rx_eeprom.at(ch).vref = static_cast<uint8_t>((buffer.at(ch * 6 + 4) & 0b10000000) >> 7);
		_rx_eeprom.at(ch).pd   = static_cast<uint8_t>((buffer.at(ch * 6 + 4) & 0b01100000) >> 5);
		_rx_eeprom.at(ch).gain = static_cast<uint8_t>((buffer.at(ch * 6 + 4) & 0b00010000) >> 4);
		_rx_eeprom.at(ch).data =
			static_cast<uint16_t>(((buffer.at(ch * 6 + 4) & 0b00001111) << 8) | buffer.at(ch * 6 + 5));
	}
}
