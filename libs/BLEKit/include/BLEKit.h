// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_KIT_H_
#define _LEKA_OS_LIB_BLE_KIT_H_

#include "ble/BLE.h"

namespace leka {

class BLEKit
{
  public:
	BLEKit() = default;

  private:
	BLE &_ble = BLE::Instance();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_KIT_H_
