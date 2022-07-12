// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "BM64Converter.h"

auto BM64::getCommand(std::span<uint8_t> cmd, size_t cmd_length, std::span<uint8_t> buffer) -> size_t
{
	size_t frame_length = 3 + cmd_length + 1;
	uint8_t checksum	= 0x00;

	/* HEAD - START */
	buffer[0] = BM64::Command::SINC_WORD;	// Not included in checksum

	/* HEAD - LENGTH (2 BYTES) */
	buffer[1] = static_cast<uint8_t>((static_cast<uint16_t>(cmd_length >> 8)));
	buffer[2] = static_cast<uint8_t>(cmd_length);
	checksum -= (buffer[1] + buffer[2]);

	/* MID - OP Code & DATA - PARAMETER */
	for (size_t i = 0; i < cmd_length; i++) {
		buffer[i + 3] = cmd[i];
		checksum -= buffer[i + 3];
	}

	/* CRC - CHECKSUM */
	buffer[frame_length - 1] = checksum;

	return frame_length;
}

// void BM64::convertResponse() {}
