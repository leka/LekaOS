// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

using namespace leka;

auto FirmwareKit::loadUpdate(FirmwareVersion &version) -> bool
{
	auto full_path = std::array<char, 64> {};
	snprintf(full_path.data(), std::size(full_path), "%s", _path_format);
	snprintf(full_path.data(), std::size(full_path), full_path.data(), version.major, version.minor, version.revision);

	return loadUpdate(full_path.data());
}

auto FirmwareKit::loadUpdate(const char *path) -> bool
{
	if (auto is_open = _file.open(path); is_open) {
		auto address = uint32_t {0x0};
		auto buffer	 = std::array<uint8_t, 256> {};

		_flash.erase();

		while (auto bytes_read = _file.read(buffer.data(), std::size(buffer))) {
			_flash.write(address, buffer, bytes_read);
			address += bytes_read;
		}

		_file.close();
		return true;
	}

	return false;
}
