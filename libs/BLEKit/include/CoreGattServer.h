// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "ble/BLE.h"
#include "ble/GattServer.h"

#include "CoreGattServerEventHandler.h"
#include "internal/BLEService.h"

namespace leka {

class CoreGattServer
{
  public:
	explicit CoreGattServer(ble::GattServer &gatt_server) : _gatt_server(gatt_server) {}

	void setEventHandler();
	void setServices(std::span<interface::BLEService *> services);

  private:
	void write(GattAttribute::Handle_t characteristic_updated, std::span<const uint8_t> data);

	ble::GattServer &_gatt_server;
	leka::CoreGattServerEventHandler _gatt_server_event_handler;
};

}	// namespace leka
