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
	// ! first line is handlE, second line is handlER --> high risk of confusion
	// ! it would be better to use handlE for bother, with a tuple for the second line
	using data_received_handle_t = GattWriteCallbackParams;
	using data_to_send_handler_t =
		std::function<void(GattAttribute::Handle_t characteristic_updated, const uint8_t *data, uint16_t n_data_bytes)>;

	BLEService(const UUID &uuid, std::span<GattCharacteristic *> characteristics)
		: GattService(uuid, characteristics.data(), std::size(characteristics)) {};

	virtual void onDataReceived(const data_received_handle_t &handle)	   = 0;
	virtual void onDataReadyToSend(const data_to_send_handler_t &function) = 0;
};

}	// namespace leka::interface
