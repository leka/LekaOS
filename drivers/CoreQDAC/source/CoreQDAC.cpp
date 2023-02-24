// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreQDAC.h"
#include <algorithm>
#include <array>

#include "rtos/ThisThread.h"

#include "LogKit.h"
#include "MemoryUtils.h"

using namespace leka;
using namespace std::chrono_literals;

void CoreQDACMCP4728::init()
{
	setVoltageReference(mcp4728::data::voltage_reference::Vdd);
	setPowerDown(mcp4728::data::power_down::normal);
	setGain(mcp4728::data::gain::x1::all);
}

void CoreQDACMCP4728::write(uint8_t channel, uint16_t data)
{
	const auto *it = std::find(std::begin(_channels), std::end(_channels), channel);

	if (it != std::end(_channels)) {
		_tx_registers.at(channel).data = data;
		// writeInputRegisters();
		writeEEPROMRegisters(channel);
	}
}

auto CoreQDACMCP4728::read(uint8_t channel) -> uint16_t
{
	if (const auto *it = std::find(std::begin(_channels), std::end(_channels), channel); it != std::end(_channels)) {
		readInputRegisters();
		return _rx_registers.at(channel).data;
	}

	return 0;
}

void CoreQDACMCP4728::writeInputRegisters()
{
	const auto number_of_bytes_per_channel = uint8_t {3};
	const auto command_size				   = number_of_bytes_per_channel * _channels.size();
	auto command						   = std::array<uint8_t, command_size> {};

	auto compute_first_byte_for_channel = [&command, this](auto channel) {
		command.at(channel * 3) = static_cast<uint8_t>(mcp4728::command::multi_write | ((0x03 & channel) << 1));
	};

	auto compute_second_byte_for_channel = [&command, this](auto channel) {
		command.at(channel * 3 + 1) = static_cast<uint8_t>(
			_tx_registers.at(channel).vref << 7 | _tx_registers.at(channel).pd << 5 |
			_tx_registers.at(channel).gain << 4 | (0x0F & utils::memory::getHighByte(_tx_registers.at(channel).data)));
	};

	auto compute_third_byte_for_channel = [&command, this](auto channel) {
		command.at(channel * 3 + 2) = utils::memory::getLowByte(_tx_registers.at(channel).data);
	};

	for (auto channel: _channels) {
		compute_first_byte_for_channel(channel);
		compute_second_byte_for_channel(channel);
		compute_third_byte_for_channel(channel);
	}

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::writeEEPROMRegisters(uint8_t channel)
{
	const auto number_of_bytes_per_channel = uint8_t {3};
	const auto command_size				   = number_of_bytes_per_channel;
	auto command						   = std::array<uint8_t, command_size> {};

	auto compute_first_byte_for_channel = [&command, this](auto channel) {
		command.at(0) = static_cast<uint8_t>(mcp4728::command::single_write | ((0x03 & channel) << 1));
	};

	auto compute_second_byte_for_channel = [&command, this](auto channel) {
		command.at(1) = static_cast<uint8_t>(_tx_registers.at(channel).vref << 7 | _tx_registers.at(channel).pd << 5 |
											 _tx_registers.at(channel).gain << 4 |
											 (0x0F & utils::memory::getHighByte(_tx_registers.at(channel).data)));
	};

	auto compute_third_byte_for_channel = [&command, this](auto channel) {
		command.at(2) = utils::memory::getLowByte(_tx_registers.at(channel).data);
	};

	compute_first_byte_for_channel(channel);
	compute_second_byte_for_channel(channel);
	compute_third_byte_for_channel(channel);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::setVoltageReference(uint8_t data)
{
	auto compute_vref_for_channel = [data](auto channel) {
		auto vref = 1 & (data >> (3 - channel));
		return vref;
	};

	auto set_vref_for_all_channels = [&] {
		for (auto channel: _channels) {
			auto vref					   = compute_vref_for_channel(channel);
			_tx_registers.at(channel).vref = static_cast<uint8_t>(vref);
		}
	};

	set_vref_for_all_channels();

	auto command = static_cast<uint8_t>(mcp4728::command::set_vref | (0x0F & data));

	_i2c.write(_address, &command, 1, false);
}

void CoreQDACMCP4728::setPowerDown(uint8_t data)
{
	auto compute_power_down_for_channel = [data](auto channel) {
		auto power_down = 3 & (data >> (6 - 2 * channel));
		return power_down;
	};

	auto set_power_down_for_all_channels = [&] {
		for (auto channel: _channels) {
			auto power_down				 = compute_power_down_for_channel(channel);
			_tx_registers.at(channel).pd = static_cast<uint8_t>(power_down);
		}
	};

	set_power_down_for_all_channels();

	auto command = std::array<uint8_t, 2> {};

	command.at(0) = static_cast<uint8_t>(mcp4728::command::set_power_down | (0xF0 & data) >> 4);
	command.at(1) = static_cast<uint8_t>((0x0F & data) << 4);

	_i2c.write(_address, command.data(), command.size(), false);
}

void CoreQDACMCP4728::setGain(uint8_t data)
{
	auto compute_gain_for_channel = [data](auto channel) {
		auto gain = 1 & (data >> (3 - channel));
		return gain;
	};

	auto set_gain_for_all_channels = [&] {
		for (auto channel: _channels) {
			auto gain					   = compute_gain_for_channel(channel);
			_tx_registers.at(channel).gain = static_cast<uint8_t>(gain);
		}
	};
	set_gain_for_all_channels();
	auto command = static_cast<uint8_t>(mcp4728::command::set_gain | (0x0F & data));
	_i2c.write(_address, &command, 1, false);
}

void CoreQDACMCP4728::readInputRegisters()
{
	auto buffer = std::array<uint8_t, mcp4728::command::read::buffer_size> {};

	_i2c.read(_address, buffer.data(), buffer.size(), false);

	auto compute_vref_for_channel = [&buffer](auto channel) {
		auto vref = (buffer.at(channel * 6 + 1) & 0x80) >> 7;
		return vref;
	};

	auto compute_power_down_for_channel = [&buffer](auto channel) {
		auto power_down = (buffer.at(channel * 6 + 1) & 0x60) >> 5;
		return power_down;
	};

	auto compute_gain_for_channel = [&buffer](auto channel) {
		auto gain = (buffer.at(channel * 6 + 1) & 0x10) >> 4;
		return gain;
	};

	auto compute_data_for_channel = [&buffer](auto channel) {
		auto data = ((buffer.at(channel * 6 + 1) & 0x0F) << 8) | buffer.at(channel * 6 + 2);
		return data;
	};

	auto set_input_data_for_all_channels = [&] {
		for (auto channel: _channels) {
			_rx_registers.at(channel).vref = static_cast<uint8_t>(compute_vref_for_channel(channel));
			_rx_registers.at(channel).pd   = static_cast<uint8_t>(compute_power_down_for_channel(channel));
			_rx_registers.at(channel).gain = static_cast<uint8_t>(compute_gain_for_channel(channel));
			_rx_registers.at(channel).data = static_cast<uint16_t>(compute_data_for_channel(channel));
		}
	};

	set_input_data_for_all_channels();

	log_info("Raw QDAC registers:");
	for (int i = 0; i < 4; i++) {
		log_info("(%x) Channel %x (_input): %2x%2x%2x", _address, i, buffer.at(i * 6 + 0), buffer.at(i * 6 + 1),
				 buffer.at(i * 6 + 2));
		log_info("(%x) Channel %x (eeprom): %2x%2x%2x", _address, i, buffer.at(i * 6 + 3), buffer.at(i * 6 + 4),
				 buffer.at(i * 6 + 5));
	}
	log_info("Compiled QDAC registers:");
	for (int channel = 0; channel < 4; channel++) {
		log_info("(%x) Channel #%i: VREF: %x, PD: %x, Gain: %x, Data: %lx", _address, channel,
				 _rx_registers.at(channel).vref, _rx_registers.at(channel).pd, _rx_registers.at(channel).gain,
				 _rx_registers.at(channel).data);
	}
	rtos::ThisThread::sleep_for(40ms);
}
