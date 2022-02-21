// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_KIT_H_
#define _LEKA_OS_LIB_BLE_KIT_H_

#include "BLEServiceBattery.h"
#include "ble/BLE.h"

#include "CoreEventQueue.h"
#include "CoreGap.h"
#include "CoreGattServer.h"

namespace leka {

class BLEKit
{
  public:
	BLEKit() = default;

	void setServices(std::span<interface::BLEService *> const &services);

	void init();

  private:
	// ? mbed::BLE specific function
	void processEvents(BLE::OnEventsToProcessCallbackContext *context);

	CoreEventQueue _event_queue {};

	BLE &_ble = BLE::Instance();
	CoreGap _core_gap {_ble.gap()};
	CoreGattServer _core_gatt_server {_ble.gattServer()};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_KIT_H_
