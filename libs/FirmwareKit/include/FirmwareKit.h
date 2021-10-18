// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_FIRMWARE_KIT_H_
#define _LEKA_OS_LIB_FIRMWARE_KIT_H_

#include "interface/drivers/FlashMemory.h"
#include "interface/platform/File.h"

namespace leka {

class FirmwareKit
{
  public:
	explicit FirmwareKit(interface::FlashMemory &flash) : _update_container(flash) {};

	void loadUpdate(interface::File &file, const char *path);

  private:
	interface::FlashMemory &_update_container;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_FIRMWARE_KIT_H_
