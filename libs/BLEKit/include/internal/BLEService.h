// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_
#define _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_

#include <span>

#include "ble/gatt/GattCharacteristic.h"
#include "ble/gatt/GattService.h"

namespace leka::interface {

class BLEService : public GattService
{
  public:
	using update_data_function_t =
		std::function<void(GattAttribute::Handle_t characteristic_updated, const uint8_t *data, uint16_t n_data_bytes)>;

	BLEService(const UUID &uuid, std::span<GattCharacteristic *> characteristics)
		: GattService(uuid, characteristics.data(), std::size(characteristics)) {};

	[[nodiscard]] auto getCharacteristicCount() const -> uint8_t { return GattService::getCharacteristicCount(); };
	auto getCharacteristic(uint8_t index) -> GattCharacteristic * { return GattService::getCharacteristic(index); };

	virtual void onDataWritten(const GattWriteCallbackParams &params) = 0;

	virtual void registerUpdateDataFunction(update_data_function_t const &function) = 0;
	virtual void updateData()														= 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_
