
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "RFIDKit.h"
#include <cstdint>

namespace leka {

void RFIDKit::sendREQA()
{
	std::array<uint8_t, 2> array {};

	commandToArray(command_requestA, array);

	_rfid_reader.send(array);
}

void RFIDKit::sendReadRegister8()
{
	std::array<uint8_t, 3> array {};

	commandToArray(command_read_register_8, array);

	_rfid_reader.send(array);
}

void RFIDKit::read() {}

}	// namespace leka
