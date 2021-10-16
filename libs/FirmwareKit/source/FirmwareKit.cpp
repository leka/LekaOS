// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"
#include <vector>

using namespace leka;

void FirmwareKit::loadUpdate(interface::File &file)
{
	if (file.is_open()) {
		auto address			   = uint32_t {0x0};
		constexpr auto packet_size = std::size_t {256};

		std::vector<uint8_t> buffer {};
		buffer.resize(packet_size);

		while (auto packet_read = file.read(buffer.data(), std::size(buffer))) {
			_update_container.write(address, buffer, packet_read);
			address += packet_read;
		}

		file.close();
	}
}
