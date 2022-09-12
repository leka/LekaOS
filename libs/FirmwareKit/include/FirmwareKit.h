// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
#include <string>

#include "FileManagerKit.h"
#include "interface/drivers/FirmwareUpdate.h"
#include "interface/drivers/FlashMemory.h"

namespace leka {

class FirmwareKit : public interface::FirmwareUpdate
{
  public:
	struct Config {
		const char *os_version_path;
		const char *bin_path_format;
	};

	static constexpr auto DEFAULT_CONFIG =
		Config {.os_version_path = "fs/sys/os-version", .bin_path_format = "/fs/usr/os/LekaOS-%i.%i.%i.bin"};

	explicit FirmwareKit(interface::FlashMemory &flash, Config config) : _flash(flash), _config(config)
	{
		// nothing do to
	}

	auto getCurrentVersion() -> leka::FirmwareVersion final;

	auto loadUpdate(const leka::FirmwareVersion &version) -> bool final;

  private:
	auto getCurrentVersionFromFile() -> leka::FirmwareVersion;

	auto loadUpdate(const char *path) -> bool;

	interface::FlashMemory &_flash;
	FileManagerKit::File _file {};

	const Config _config;
};

}	// namespace leka
