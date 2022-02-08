// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_SERVICE_BATTERY_H_
#define _LEKA_OS_LIB_BLE_SERVICE_BATTERY_H_

#include "internal/BLEService.h"

namespace leka {

class BLEServiceBattery : public interface::BLEService
{
	const static uint16_t SERVICE_BATTERY_UUID						 = GattService::UUID_BATTERY_SERVICE;
	const static uint16_t BATTERY_LEVEL_WRITABLE_CHARACTERISTIC_UUID = GattCharacteristic::UUID_BATTERY_LEVEL_CHAR;

  public:
	BLEServiceBattery() : interface::BLEService(SERVICE_BATTERY_UUID, _characteristic_table) {};

	void setBatteryLevel(uint8_t value)
	{
		if (value <= 100) {
			_battery_level_characteristic_value = value;
		}
		auto data = std::make_tuple(_battery_level_writable_characteristic.getValueHandle(),
									std::span(&_battery_level_characteristic_value, 1));
		sendData(data);
	}

	void onDataReceived(const data_received_handle_t &params) final {
		// do nothing
	};

  private:
	data_to_send_handle_t send_data_function;

	uint8_t _battery_level_characteristic_value {};
	ReadOnlyGattCharacteristic<uint8_t> _battery_level_writable_characteristic {
		BATTERY_LEVEL_WRITABLE_CHARACTERISTIC_UUID, &_battery_level_characteristic_value,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

	std::array<GattCharacteristic *, 1> _characteristic_table {&_battery_level_writable_characteristic};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_SERVICE_BATTERY_H_
