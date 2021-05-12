// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

namespace leka {

LKCoreRFID::LKCoreRFID(interface::BufferedSerial &interface) : _interface(interface) {}

auto LKCoreRFID::writeProtocol() -> void
{
	const uint8_t buffer_size				  = 4;
	const uint8_t command_buffer[buffer_size] = {0x02, 0x02, 0x02, 0x00};

	_interface.write(command_buffer, buffer_size);
}

auto LKCoreRFID::setGain() -> void
{
	const uint8_t buffer_size				  = 6;
	const uint8_t command_buffer[buffer_size] = {0x09, 0x04, 0x68, 0x01, 0x01, 0xD1};

	_interface.write(command_buffer, buffer_size);
}

auto LKCoreRFID::checkSensorSet() -> bool
{
	const uint8_t buffer_size = 2;
	uint8_t buffer[buffer_size];

	_interface.read(buffer, buffer_size);

	if (buffer[0] == 0x00 && buffer[1] == 0x00) {
		return true;
	}

	return false;
}

auto LKCoreRFID::sendREQA() -> void
{
	const uint8_t buffer_size				  = 4;
	const uint8_t command_buffer[buffer_size] = {0x04, 0x02, 0x26, 0x07};

	_interface.write(command_buffer, buffer_size);
}

auto LKCoreRFID::checkATQA() -> bool
{
	const uint8_t buffer_size = 7;
	uint8_t buffer[buffer_size];

	_interface.read(buffer, buffer_size);

	if ((buffer[2] == 0x04) && (buffer[3] == 0x00)) {
		return true;
	}

	return false;
}

auto LKCoreRFID::sendCL1() -> void
{
	const uint8_t buffer_size				  = 5;
	const uint8_t command_buffer[buffer_size] = {0x04, 0x03, 0x93, 0x20, 0x08};

	_interface.write(command_buffer, buffer_size);
}

auto LKCoreRFID::receiveUID1() -> void
{
	const uint8_t buffer_size = 10;
	uint8_t buffer[buffer_size];

	_interface.read(buffer, buffer_size);
	memcpy(_rfid_tag.UID, buffer + 2, 4);
}

auto LKCoreRFID::sendUID1() -> void
{
	const uint8_t buffer_size				  = 10;
	const uint8_t command_buffer[buffer_size] = {0x04, 0x08, 0x93, 0x70, 0x88, 0x04, 0x61, 0xD5, 0x38, 0x28};

	_interface.write(command_buffer, buffer_size);
}
}	// namespace leka
