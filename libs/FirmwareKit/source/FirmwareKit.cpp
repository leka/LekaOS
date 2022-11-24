// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "FirmwareKit.h"

#include "os_version.h"
#include "semver/semver.hpp"

using namespace leka;

auto FirmwareKit::getCurrentVersion() -> Version
{
	auto semversion = semver::version {OS_VERSION};
	return Version {.major = semversion.major, .minor = semversion.minor, .revision = semversion.patch};
}

auto FirmwareKit::getPathOfVersion(const Version &version) const -> std::filesystem::path
{
	auto path_buffer = std::array<char, 64> {};
	snprintf(path_buffer.data(), std::size(path_buffer), _config.bin_path_format, version.major, version.minor,
			 version.revision);

	auto path = std::filesystem::path {path_buffer.begin()};

	return path;
}

auto FirmwareKit::loadUpdate(const Version &version) -> bool
{
	auto path = getPathOfVersion(version);

	return loadUpdate(path);
}

auto FirmwareKit::loadUpdate(const std::filesystem::path &path) -> bool
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
