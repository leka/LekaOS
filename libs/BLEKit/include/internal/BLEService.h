// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "ble/gatt/GattCharacteristic.h"
#include "ble/gatt/GattService.h"

namespace leka::interface {

class BLEService : public GattService
{
  public:
	BLEService(const UUID &uuid, std::span<GattCharacteristic *> characteristics)
		: GattService(uuid, characteristics.data(), std::size(characteristics)) {};

	using data_received_handle_t = GattWriteCallbackParams;
	using data_to_send_handle_t	 = std::tuple<GattAttribute::Handle_t, std::span<const uint8_t>>;

	virtual void onDataReceived(const data_received_handle_t &handle) = 0;

	void onDataReadyToSend(const std::function<void(const data_to_send_handle_t &)> &callback)
	{
		_callback_on_data_ready_to_send = callback;
	};

	void sendData(const data_to_send_handle_t &handle) const { _callback_on_data_ready_to_send(handle); }

  private:
	std::function<void(const data_to_send_handle_t &)> _callback_on_data_ready_to_send {};
};

}	// namespace leka::interface
