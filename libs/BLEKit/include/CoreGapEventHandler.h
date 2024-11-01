// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

#include "ble/BLE.h"
#include "ble/Gap.h"

namespace leka {

class CoreGapEventHandler : public ble::Gap::EventHandler
{
  public:
	explicit CoreGapEventHandler() = default;

	void registerStartAdvertising(std::function<void()> const &function);

	void onInitializationComplete(BLE::InitializationCompleteCallbackContext *params) const;

	void onConnectionComplete(ble::ConnectionCompleteEvent const &event) override;
	void onDisconnectionComplete(ble::DisconnectionCompleteEvent const &event) override;
	void onAdvertisingEnd(ble::AdvertisingEndEvent const &event) override;

	void onConnectionCallback(const std::function<void(ble::connection_handle_t handle)> &callback);
	void onDisconnectionCallback(const std::function<void()> &callback);
	[[nodiscard]] auto isConnected() const -> bool;

	[[nodiscard]] auto getConnectionHandle() const -> ble::connection_handle_t;

  private:
	bool is_connected = false;

	std::function<void()> _start_advertising {};

	std::function<void(ble::connection_handle_t handle)> _on_connection_callback {};
	std::function<void()> _on_disconnection_callback {};

	ble::connection_handle_t _handle;
};

}	// namespace leka
