// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

#include "semver/semver.hpp"

using namespace leka;

auto FirmwareKit::getCurrentVersion() -> FirmwareVersion
{
	return getCurrentVersionFromFile();
}

auto FirmwareKit::getCurrentVersionFromFile() -> FirmwareVersion
{
	auto file_content = std::array<char, 16> {};

	if (auto is_not_open = !_file.open(os_version_path); is_not_open) {
		return FirmwareVersion {.major = 1, .minor = 0, .revision = 0};
	}

	_file.read(file_content);
	_file.close();

	std::replace(std::begin(file_content), std::end(file_content), '\n', '\0');
	auto semversion = semver::version {file_content.data()};

	return FirmwareVersion {.major = semversion.major, .minor = semversion.minor, .revision = semversion.patch};
}

auto FirmwareKit::loadUpdate(const FirmwareVersion &version) -> bool
{
	auto path = std::array<char, 64> {};
	snprintf(path.data(), std::size(path), _path_format, version.major, version.minor, version.revision);

	return loadUpdate(path.data());
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
