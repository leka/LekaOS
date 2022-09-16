// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

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
	void onDataRead(const GattReadCallbackParams &params) override;

  private:
	std::span<interface::BLEService *> _services {};
};

}	// namespace leka
