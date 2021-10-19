// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

using namespace leka;

void FirmwareKit::setDefaultPath(const lstd::span<char> &path)
{
	_default_path = path;
}

auto FirmwareKit::loadUpdate(FirmwareVersion &version) -> bool
{
	lstd::span<char> full_path = _default_path;
	snprintf(full_path.data() + _default_path.size(), 28, "LekaOS-%i.%i.%i.bin", version.major, version.minor,
			 version.revision);

	return loadUpdate(full_path);
}

auto FirmwareKit::loadUpdate(lstd::span<char> &path) -> bool
{
	if (auto is_open = _file.open(path.data()); is_open) {
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
