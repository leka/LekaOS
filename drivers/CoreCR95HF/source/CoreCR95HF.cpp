
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

namespace leka {

CoreCR95HF::CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {}

void CoreCR95HF::send(uint8_t *data, const size_t size)
{
	const size_t command_size = size + 3;

	formatCommand(data, _send_buffer.data(), size);

	_serial.write(_send_buffer.data(), command_size);
}

void CoreCR95HF::formatCommand(const uint8_t *command_iso, uint8_t *command_send, size_t size_of_command_iso) const
{
	command_send[0] = CR95HF::commands::send_reveive;
	command_send[1] = static_cast<uint8_t>(size_of_command_iso) + 1;

	for (unsigned int i = 0; i < size_of_command_iso; ++i) {
		command_send[i + 2] = command_iso[i];
	}
	command_send[size_of_command_iso + 2] = findCorrespondingFlag(command_iso);
}

auto CoreCR95HF::findCorrespondingFlag(const uint8_t *command_iso) const -> uint8_t
{
	if (command_iso == nullptr) {
		return 0;
	}
	if (command_iso[0] == ISO14443_command::request_A) {
		return CR95HF::flags::seven_significant_bits;
	}
	if (command_iso[0] == ISO14443_command::read_register_8[0]) {
		return CR95HF::flags::eight_significant_bits_with_crc;
	}
	return 0;
}

void CoreCR95HF::receive(uint8_t *data, size_t size)
{
	_serial.read(_receive_buffer.data(), size);

	for (unsigned int i = 0; i < size; ++i) {
		data[i] = _receive_buffer[i];
	}
}

void CoreCR95HF::setProcoleISO14443()
{
	std::array<uint8_t, 4> set_protocol_ISO14443_command {0x02, 0x02, 0x02, 0x00};
	_serial.write(set_protocol_ISO14443_command.data(), 4);
}

void CoreCR95HF::setGainAndModulation()
{
	std::array<uint8_t, 6> set_gain_and_modulation_command {0x09, 0x04, 0x68, 0x01, 0x01, 0xD1};
	_serial.write(set_gain_and_modulation_command.data(), 6);
}

}	// namespace leka
