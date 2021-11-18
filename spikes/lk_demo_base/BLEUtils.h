// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_BLE_UTILS_H_
#define _LEKA_OS_BLE_UTILS_H_

#include "events/EventQueue.h"
#include "rtos/EventFlags.h"

#include "BLEGap.h"
#include "BLEGattServer.h"

namespace leka {

class BLEUtils
{
  public:
	explicit BLEUtils(rtos::EventFlags &event_flags)
		: _event_flags(event_flags), _ble {BLE::Instance()}, _ble_gap {_ble, _event_queue}, _ble_server(_ble) {};

	void setDeviceName(lstd::span<const char> name);

	void startAdvertising();

  private:
	rtos::EventFlags &_event_flags;
	events::EventQueue _event_queue {16 * EVENTS_EVENT_SIZE};

	BLE &_ble;
	BLEGap _ble_gap;
	BLEGattServer _ble_server;
};

}	// namespace leka

#endif	 // _LEKA_OS_BLE_UTILS_H_
