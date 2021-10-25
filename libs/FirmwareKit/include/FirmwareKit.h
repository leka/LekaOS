// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FIRMWARE_KIT_H_
#define _LEKA_OS_LIB_FIRMWARE_KIT_H_

#include <functional>
#include <string>

#include "FileSystemKit.h"
#include "interface/drivers/FirmwareUpdate.h"
#include "interface/drivers/FlashMemory.h"

namespace leka {

class FirmwareKit : public interface::FirmwareUpdate
{
  public:
	explicit FirmwareKit(interface::FlashMemory &flash, const char *format = "/fs/os/LekaOS-%i.%i.%i.bin")
		: _flash(flash), _path_format(format) {};

	auto loadUpdate(leka::FirmwareVersion &version) -> bool final;

  private:
	auto loadUpdate(const char *path) -> bool;

	interface::FlashMemory &_flash;
	FileSystemKit::File _file {};

	const char *_path_format;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FIRMWARE_KIT_H_
