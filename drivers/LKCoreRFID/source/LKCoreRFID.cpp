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

	if (buffer[0] == 0 && buffer[1] == 0) {
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

bool LKCoreRFID::isDataLengthOk(uint8_t length)
{
	if (length != 8) {
		return false;
	}
	return true;
}

bool LKCoreRFID::isResultCodeOk(uint8_t command)
{
	if (command != 0x80) {
		return false;
	}
	return true;
}

auto LKCoreRFID::RFIDMessageIntoStruct(uint8_t *tag_value, RFIDTag &rfid_tag) -> bool
{
	if (!isResultCodeOk(tag_value[0])) {
		return false;
	};
	rfid_tag.result_code = tag_value[0];

	if (!isDataLengthOk(tag_value[1])) {
		return false;
	};
	rfid_tag.length = tag_value[1];

	for (int i = 0; i < 4; ++i) {
		rfid_tag.id[i] = tag_value[i + 2];
	}

	rfid_tag.check_sum	   = tag_value[6];
	rfid_tag.checks		   = tag_value[7];
	rfid_tag.collisionbyte = tag_value[8];
	rfid_tag.collisionbit  = tag_value[9];

	return true;
}

}	// namespace leka
