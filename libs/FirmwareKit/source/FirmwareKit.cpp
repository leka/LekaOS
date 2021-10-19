// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

using namespace leka;

void FirmwareKit::setDefaultPath(std::string path)
{
	_default_path = path;
}

void FirmwareKit::loadUpdate(FirmwareVersion &version)
{
	std::string file_name;
	snprintf(file_name.data(), 100, "LekaOS-%i.%i.%i.bin", version.major, version.minor, version.revision);

	std::string full_path = _default_path + file_name;

	loadUpdate(full_path);
}

void FirmwareKit::loadUpdate(std::string path)
{
	if (auto is_open = _file.open(path.data()); is_open) {
		auto address			   = uint32_t {0x0};
		constexpr auto packet_size = std::size_t {256};

		std::array<uint8_t, packet_size> buffer {};

		while (auto packet_read = _file.read(buffer.data(), std::size(buffer))) {
			_update_container.write(address, buffer, packet_read);
			address += packet_read;
		}

		_file.close();
	}
}
