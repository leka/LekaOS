// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <filesystem>
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
		const char *bin_path_format;
		const char *factory_path;
	};

	static constexpr auto DEFAULT_CONFIG =
		Config {.bin_path_format = "/fs/usr/os/LekaOS-%i.%i.%i.bin", .factory_path = "/fs/usr/os/LekaOS-factory.bin"};

	explicit FirmwareKit(interface::FlashMemory &flash, Config config) : _flash(flash), _config(config)
	{
		// nothing do to
	}

	auto getCurrentVersion() -> Version final;

	auto isVersionAvailable(const Version &version) -> bool final;
	auto loadFirmware(const Version &version) -> bool final;
	auto loadFactoryFirmware() -> bool final;

  private:
	[[nodiscard]] auto getPathOfVersion(const Version &version) const -> std::filesystem::path;

	auto load(const std::filesystem::path &path) -> bool;

	interface::FlashMemory &_flash;
	FileManagerKit::File _file {};

	const Config _config;
};

}	// namespace leka
