// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_GATT_SERVER_H_
#define _LEKA_OS_LIB_BLE_GATT_SERVER_H_

#include "events/EventQueue.h"

#include "ble/BLE.h"
#include "ble/GattServer.h"

#include "cxxsupport/lstd_span"
#include "interface/drivers/BLEService.h"

namespace leka {

class BLEGattServer : public ble::GattServer::EventHandler
{
  public:
	explicit BLEGattServer(BLE &ble) : _ble(ble), _server(_ble.gattServer()) {};

	void addServices(lstd::span<interface::BLEService *> services);

	void start(BLE &ble, events::EventQueue &event_queue);

  private:
	void onDataWritten(const GattWriteCallbackParams &params) override;

	BLE &_ble;
	ble::GattServer &_server;

	lstd::span<interface::BLEService *> _services {};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_GATT_SERVER_H_
