// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_BLE_UTILS_H_
#define _LEKA_OS_BLE_UTILS_H_

#include "events/EventQueue.h"
#include "rtos/EventFlags.h"

#include "BLEGap.h"
#include "BLEGattServer.h"
#include "BLEServiceBattery.h"
#include "BLEServiceMonitoring.h"

namespace leka {

class BLEUtils
{
  public:
	explicit BLEUtils(rtos::EventFlags &event_flags)
		: _event_flags(event_flags),
		  _ble {BLE::Instance()},
		  _ble_gap {_ble, _event_queue},
		  _ble_server(_ble),
		  _ble_service_monitoring(_event_flags),
		  _ble_service_battery(_event_flags) {};

	void setDeviceName(lstd::span<const char> name);

	void setBatteryLevel(float value);

	auto getPing() -> bool;
	auto getRebootInstruction() -> bool;
	auto getLedsIntensity() -> uint8_t;
	auto getLCDIntensity() -> uint8_t;

	void startAdvertising();

  private:
	rtos::EventFlags &_event_flags;
	events::EventQueue _event_queue {16 * EVENTS_EVENT_SIZE};

	BLE &_ble;
	BLEGap _ble_gap;
	BLEGattServer _ble_server;

	BLEServiceMonitoring _ble_service_monitoring;
	BLEServiceBattery _ble_service_battery;
};

}	// namespace leka

#endif	 // _LEKA_OS_BLE_UTILS_H_
