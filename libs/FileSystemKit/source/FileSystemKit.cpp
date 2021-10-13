// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FileSystemKit.h"
#include <vector>

using namespace leka;

void FileSystemKit::sendFileToFlash(File &file_sent, interface::FlashMemory &flash)
{
	if (file_sent.is_open()) {
		uint32_t address		 = 0x0;
		const size_t packet_size = 0x100;
		auto packet_read		 = packet_size;
		std::vector<uint8_t> buffer {};
		buffer.resize(packet_size);

		while (packet_read != 0) {
			packet_read = file_sent.read(buffer.data(), packet_size);
			flash.write(address, buffer, packet_read);
			address += packet_read;
		}

		file_sent.close();
	}
}
