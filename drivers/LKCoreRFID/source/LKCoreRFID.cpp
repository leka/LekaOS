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

}	// namespace leka
