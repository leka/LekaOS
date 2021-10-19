// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

using namespace leka;

void FirmwareKit::setDefaultPath(const std::string_view &path)
{
	std::copy(path.begin(), path.end(), _default_path.begin());
}

auto FirmwareKit::loadUpdate(FirmwareVersion &version) -> bool
{
	std::array<char, 28> file_name {};
	snprintf(file_name.data(), file_name.size(), "LekaOS-%i.%i.%i.bin", version.major, version.minor, version.revision);

	std::array<char, 128> concatene_path {};
	std::copy(_default_path.begin(), _default_path.end(), concatene_path.begin());
	std::copy(file_name.begin(), file_name.end(), concatene_path.begin() + _default_path.size());

	std::string_view full_path(concatene_path.begin(), concatene_path.size());

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
