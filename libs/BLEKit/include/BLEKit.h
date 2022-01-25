// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_KIT_H_
#define _LEKA_OS_LIB_BLE_KIT_H_

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "ble/BLE.h"

namespace leka {

class BLEKit
{
  public:
	BLEKit() = default;

	void init();

  private:
	void processEvents(BLE::OnEventsToProcessCallbackContext *event)
	{
		_event_queue.call(mbed::callback(&event->ble, &BLE::processEvents));
	}

	rtos::Thread _thread {};
	events::EventQueue _event_queue {};

	BLE &_ble = BLE::Instance();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_KIT_H_
