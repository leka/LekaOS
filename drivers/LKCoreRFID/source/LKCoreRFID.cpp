// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreRFID.h"

namespace leka {

LKCoreRFID::LKCoreRFID(mbed::BufferedSerial &interface) : _interface(interface) {}

RFIDTag LKCoreRFID::RFIDMessageIntoStruct(uint8_t *tag_value)
{
	RFIDTag decompose_value;

	uint8_t length = tag_value[1];
	if (!isDataLengthOk(length)) {
		return {0, 0, 0};
	}

	decompose_value.command = tag_value[0];
	decompose_value.length	= length;

	for (int i = 0; i < decompose_value.length; ++i) {
		decompose_value.data[i] = tag_value[i + 2];
	}

	return decompose_value;
}

// auto setProtocol() -> void
// {
// 	const uint8_t iec_14443_cmd[4]	   = {0x02, 0x02, 0x02, 0x00};
// 	const uint8_t iec_14443_cmd_length = 4;
// 	_interface.write(iec_14443_cmd, iec_14443_cmd_length);
// }

auto LKCoreRFID::isDataLengthOk(uint8_t length) -> bool
{
	if (length > _MAXIMUM_BUFFER_SIZE) {
		return false;
	}
	return true;
}

}	// namespace leka
