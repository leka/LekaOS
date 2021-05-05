// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

namespace leka {

LKCoreRFID::LKCoreRFID(interface::BufferedSerial &interface) : _interface(interface) {}

auto LKCoreRFID::writeProtocol() -> void
{
	const char iec_14443_cmd[4]		   = {0x02, 0x02, 0x02, 0x00};
	const uint8_t iec_14443_cmd_length = 4;
	_interface.write(iec_14443_cmd, iec_14443_cmd_length);
}

auto LKCoreRFID::checkProtocol() -> bool
{
	char buffer[2];
	_interface.read(buffer, 2);

	if (buffer[0] == 0 && buffer[1] == 0) {
		return true;
	}

	return false;
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

	for (int i = 0; i < 5; ++i) {
		rfid_tag.id[i] = tag_value[i + 2];
	}

	rfid_tag.checks		   = tag_value[7];
	rfid_tag.collisionbyte = tag_value[8];
	rfid_tag.collisionbit  = tag_value[9];

	return true;
}

}	// namespace leka
