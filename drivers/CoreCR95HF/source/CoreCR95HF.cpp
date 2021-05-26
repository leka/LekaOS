
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreCR95HF.h"

namespace leka {

CoreCR95HF::CoreCR95HF(interface::BufferedSerial &serial) : _serial(serial) {}

void CoreCR95HF::send(uint8_t *data, const size_t size)
{
	const size_t max_command_size = 5;
	const size_t command_size	  = size + 3;

	uint8_t command[max_command_size] {0};

	formatCommand(data, command, size);

	_serial.write(command, command_size);
}

void CoreCR95HF::formatCommand(const uint8_t *data, uint8_t *command, size_t size)
{
	command[0] = 0x04;
	command[1] = size + 1;

	for (int i = 0; i < size; ++i) {
		command[i + 2] = data[i];
	}
	command[size + 2] = findCorrespondingFlag(size);
}

const size_t CoreCR95HF::findCorrespondingFlag(size_t size)
{
	if (size == 1) {
		return 0x07;
	}
	if (size == 2) {
		return 0x28;
	}
	return 0;
}
void CoreCR95HF::receive(uint8_t *data, size_t size) {}

}	// namespace leka
