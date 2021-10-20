// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FIRMWARE_KIT_H_
#define _LEKA_OS_LIB_FIRMWARE_KIT_H_

#include <string>

#include "FileSystemKit.h"
#include "interface/drivers/FirmwareUpdate.h"
#include "interface/drivers/FlashMemory.h"

namespace leka {

using pGetFileName = void (*)(char *file_name, size_t max_file_name_size, const leka::FirmwareVersion &version);

class FirmwareKit : public interface::FirmwareUpdate
{
  public:
	explicit FirmwareKit(interface::FlashMemory &flash) : _update_container(flash) {};

	void setDefaultPath(const char *path);
	void setFileNameFormat(pGetFileName const &p_get_file_name);

	auto loadUpdate(leka::FirmwareVersion &version) -> bool final;

  private:
	auto loadUpdate(const char *path) -> bool;

	pGetFileName getFileName {};

	interface::FlashMemory &_update_container;
	FileSystemKit::File _file {};

	std::array<char, 128> _full_path {};
	int _default_path_size {};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FIRMWARE_KIT_H_
