// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_
#define _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_

#include "ble/gatt/GattCharacteristic.h"
#include "ble/gatt/GattService.h"

namespace leka {

using updateServiceFunction =
	std::function<void(GattAttribute::Handle_t characteristic_updated, const uint8_t *data, uint16_t n_data_bytes)>;

namespace interface {

	class BLEService : public GattService
	{
	  public:
		BLEService(const UUID &uuid, GattCharacteristic *characteristics[], unsigned characteristics_count)
			: GattService(uuid, characteristics, characteristics_count) {};

		virtual void onDataWritten(const GattWriteCallbackParams &params) = 0;
		virtual void updateData(updateServiceFunction &update)			  = 0;
	};

}	// namespace interface
}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_SERVICE_INTERFACE_H_
