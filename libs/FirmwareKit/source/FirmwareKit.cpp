// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

using namespace leka;

void FirmwareKit::setDefaultPath(const char *path)
{
	_default_path_size = snprintf(_full_path.begin(), _full_path.size(), "%s", path);
}

auto FirmwareKit::loadUpdate(FirmwareVersion &version) -> bool
{
	snprintf(_full_path.begin() + _default_path_size, _full_path.size() - _default_path_size, "LekaOS-%i.%i.%i.bin",
			 version.major, version.minor, version.revision);

	return loadUpdate(_full_path.data());
}

auto FirmwareKit::loadUpdate(const char *path) -> bool
{
	if (auto is_open = _file.open(path); is_open) {
		auto address			   = uint32_t {0x0};
		constexpr auto packet_size = std::size_t {256};

		std::array<uint8_t, packet_size> buffer {};

		_update_container.erase();

		while (auto packet_read = _file.read(buffer.data(), std::size(buffer))) {
			_update_container.write(address, buffer, packet_read);
			address += packet_read;
		}

		_file.close();

		return true;
	}

	return false;
}
