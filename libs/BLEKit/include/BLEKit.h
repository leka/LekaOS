// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_KIT_H_
#define _LEKA_OS_LIB_BLE_KIT_H_

#include "ble/BLE.h"

#include "CoreEventQueue.h"
#include "CoreGap.h"

namespace leka {

class BLEKit
{
  public:
	BLEKit() = default;

	void init();

  private:
	// ? mbed::BLE specific function
	void processEvents(BLE::OnEventsToProcessCallbackContext *context);

	CoreEventQueue _event_queue {};

	BLE &_ble = BLE::Instance();
	CoreGap _core_gap {_event_queue, _ble.gap()};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_KIT_H_
