// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

using namespace leka;

void FirmwareKit::setDefaultPath(const std::string_view &path)
{
	_default_path = path;
}

auto FirmwareKit::loadUpdate(FirmwareVersion &version) -> bool
{
	lstd::span<char> file_name {};
	snprintf(file_name.data(), 28, "LekaOS-%i.%i.%i.bin", version.major, version.minor, version.revision);

	std::string_view full_path(file_name.begin(), file_name.size());

	return loadUpdate(full_path);
}

auto FirmwareKit::loadUpdate(const std::string_view &path) -> bool
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
