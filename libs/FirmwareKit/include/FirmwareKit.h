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
	static constexpr auto os_version_path = "fs/sys/os-version";

  public:
	explicit FirmwareKit(interface::FlashMemory &flash, const char *format = "/fs/usr/os/LekaOS-%i.%i.%i.bin")
		: _flash(flash), _path_format(format)
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

	const char *_path_format;
};

}	// namespace leka
