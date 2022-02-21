// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_CORE_GATT_SERVER_EVENT_HANDLER_H_
#define _LEKA_OS_LIB_CORE_GATT_SERVER_EVENT_HANDLER_H_

#include <span>

#include "ble/BLE.h"
#include "ble/GattServer.h"

#include "internal/BLEService.h"

namespace leka {

class CoreGattServerEventHandler : public ble::GattServer::EventHandler
{
  public:
	explicit CoreGattServerEventHandler() = default;

	void setServices(std::span<interface::BLEService *> const &services);

	void onDataWritten(const GattWriteCallbackParams &params) override;

  private:
	std::span<interface::BLEService *> _services {};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_CORE_GATT_SERVER_EVENT_HANDLER_H_
