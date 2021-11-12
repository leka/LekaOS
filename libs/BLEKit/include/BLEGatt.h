// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_GATT_H_
#define _LEKA_OS_LIB_BLE_GATT_H_

#include <events/mbed_events.h>

#include "ble/BLE.h"

#include "LogKit.h"

class BLEGatt : ble::GattServer::EventHandler
{
	const static uint16_t SERVICE_UUID				   = 0xA000;
	const static uint16_t WRITABLE_CHARACTERISTIC_UUID = 0xA001;

  public:
	BLEGatt() : _writable_characteristic {WRITABLE_CHARACTERISTIC_UUID, _characteristic_value.begin()} {};

	void start(BLE &ble, events::EventQueue &event_queue)
	{
		std::array<GattCharacteristic *, 1> char_table {&_writable_characteristic};
		GattService example_service(SERVICE_UUID, char_table.data(), 1);

		ble.gattServer().addService(example_service);

		ble.gattServer().setEventHandler(this);
	}

  private:
	void onDataWritten(const GattWriteCallbackParams &params) override
	{
		if (params.handle == _writable_characteristic.getValueHandle()) {
			if (params.offset == 0) {
				std::fill_n(_characteristic_value.begin(), std::size(_characteristic_value), '\0');
			}
			std::copy(params.data, params.data + params.len, _characteristic_value.begin() + params.offset);
			log_info("Data received: length: %d, offset: %d, data: \n%s", params.len, params.offset,
					 _characteristic_value.data());
		}
	}

	std::array<char, 100> _characteristic_value {};
	WriteOnlyArrayGattCharacteristic<char, 100> _writable_characteristic;
};

#endif	 // _LEKA_OS_LIB_BLE_GATT_H_
