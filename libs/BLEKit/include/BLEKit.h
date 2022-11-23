// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
#include <span>

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

	void setAdvertisingData(const AdvertisingData &advertising_data);
	[[nodiscard]] auto getAdvertisingData() const -> AdvertisingData;

	void onConnectionCallback(const std::function<void()> &callback);
	void onDisconnectionCallback(const std::function<void()> &callback);

  private:
	// ? mbed::BLE specific function
	void processEvents(BLE::OnEventsToProcessCallbackContext *context);

	CoreEventQueue _event_queue {};

	AdvertisingData _advertising_data {};

	BLE &_ble = BLE::Instance();
	CoreGap _core_gap {_ble.gap()};
	CoreGattServer _core_gatt_server {_ble.gattServer()};
};

}	// namespace leka
