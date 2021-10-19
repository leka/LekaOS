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

class FirmwareKit : public interface::FirmwareUpdate
{
  public:
	explicit FirmwareKit(interface::FlashMemory &flash) : _update_container(flash) {};

	void setDefaultPath(std::string path);

	void loadUpdate(leka::FirmwareVersion &version) final;

  private:
	void loadUpdate(std::string path);

	interface::FlashMemory &_update_container;
	FileSystemKit::File _file {};

	std::string _default_path {"/fs/os/"};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FIRMWARE_KIT_H_
