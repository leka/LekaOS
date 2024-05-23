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

	void onMTUNegotiated(const std::function<void(uint16_t)> &callback);
	void onAttMtuChange(ble::connection_handle_t handle, uint16_t new_mtu) override;

  private:
	std::span<interface::BLEService *> _services {};

	std::function<void(uint16_t)> _on_mtu_negotiated_callback {};
};

}	// namespace leka
