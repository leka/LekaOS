// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVERS_FIRMWARE_UPDATE_H_
#define _LEKA_OS_INTERFACE_DRIVERS_FIRMWARE_UPDATE_H_

#include "FirmwareVersion.h"

namespace leka::interface {

class FirmwareUpdate
{
  public:
	virtual ~FirmwareUpdate() = default;

	virtual void loadUpdate(FirmwareVersion &version) = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVERS_FIRMWARE_UPDATE_H_
