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
	explicit FirmwareKit(interface::FlashMemory &flash,
						 const char *firmware_bin_path_format = "/fs/usr/os/LekaOS-%i.%i.%i.bin",
						 const char *os_version_path		  = "fs/sys/os-version")
		: _flash(flash), _firmware_bin_path_format(firmware_bin_path_format), _os_version_path(os_version_path)
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

	const char *_firmware_bin_path_format;
	const char *_os_version_path;
};

}	// namespace leka
