// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"
#include <vector>

using namespace leka;

void FirmwareKit::loadUpdate(interface::File &update_file)
{
	if (update_file.is_open()) {
		auto address			   = uint32_t {0x0};
		constexpr auto packet_size = std::size_t {256};

		std::vector<uint8_t> buffer {};
		buffer.resize(packet_size);

		auto packet_read = update_file.read(buffer.data(), std::size(buffer));
		while (packet_read != 0) {
			_update_container.write(address, buffer, packet_read);
			address += packet_read;

			packet_read = update_file.read(buffer.data(), std::size(buffer));
		}

		update_file.close();
	}
}
