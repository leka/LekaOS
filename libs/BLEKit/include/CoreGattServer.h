// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_CORE_GATT_SERVER_H_
#define _LEKA_OS_LIB_CORE_GATT_SERVER_H_

#include <span>

#include "BLEServiceBattery.h"
#include "ble/BLE.h"
#include "ble/GattServer.h"

#include "CoreGattServerEventHandler.h"
#include "interface/drivers/BLEService.h"

namespace leka {

class CoreGattServer
{
  public:
	explicit CoreGattServer(ble::GattServer &gatt_server) : _gatt_server(gatt_server) {};

	void setServer();

	void setEventHandler();
	void setServices();
	void registerServerWrite();

	void updateData();

  private:
	void serverWrite(GattAttribute::Handle_t characteristic_updated, const uint8_t *data, uint16_t n_data_bytes);

	ble::GattServer &_gatt_server;
	leka::CoreGattServerEventHandler _gatt_server_event_handler;

	BLEServiceBattery _ble_service_battery;
	std::array<interface::BLEService *, 1> _services {&_ble_service_battery};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_CORE_GATT_SERVER_H_
