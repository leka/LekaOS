// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
#include <span>
#include <tuple>

#include "ble/gatt/GattCharacteristic.h"
#include "ble/gatt/GattService.h"

namespace leka::interface {

class BLEService : public GattService
{
  public:
	BLEService(const UUID &uuid, std::span<GattCharacteristic *> characteristics)
		: GattService(uuid, characteristics.data(), std::size(characteristics))
	{
		// nothing do to
	}

	virtual ~BLEService() = default;

	using data_received_handle_t  = GattWriteCallbackParams;
	using data_requested_handle_t = GattReadCallbackParams;
	using data_to_send_handle_t	  = std::tuple<const GattCharacteristic *, std::span<const uint8_t>>;

	virtual void onDataReceived(const data_received_handle_t &handle)	= 0;
	virtual void onDataRequested(const data_requested_handle_t &handle) = 0;

	void onDataReadyToSend(const std::function<void(const data_to_send_handle_t &)> &callback)
	{
		_callback_on_data_ready_to_send = callback;
	}

	void sendData(const data_to_send_handle_t &handle) const { _callback_on_data_ready_to_send(handle); }

  private:
	std::function<void(const data_to_send_handle_t &)> _callback_on_data_ready_to_send {};
};

}	// namespace leka::interface
