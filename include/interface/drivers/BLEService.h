// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_
#define _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_

#include <span>

#include "ble/gatt/GattCharacteristic.h"
#include "ble/gatt/GattService.h"

namespace leka {

using updateServiceFunction =
	std::function<void(GattAttribute::Handle_t characteristic_updated, const uint8_t *data, uint16_t n_data_bytes)>;

namespace interface {

	class BLEService : public GattService
	{
	  public:
		BLEService(const UUID &uuid, std::span<GattCharacteristic *> characteristics, unsigned characteristics_count)
			: GattService(uuid, characteristics.data(), characteristics_count) {};

		virtual void onDataWritten(const GattWriteCallbackParams &params) = 0;
		virtual void updateData(updateServiceFunction &update)			  = 0;

		virtual auto getCharacteristicCount() -> uint8_t { return GattService::getCharacteristicCount(); };
		virtual auto getCharacteristic(uint8_t index) -> GattCharacteristic * { return getCharacteristic(index); };
	};

}	// namespace interface
}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_
