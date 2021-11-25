// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_SERVICE_BATTERY_H_
#define _LEKA_OS_LIB_BLE_SERVICE_BATTERY_H_

#include "rtos/EventFlags.h"

#include "Flags.h"
#include "interface/drivers/BLEService.h"

namespace leka {

class BLEServiceBattery : public interface::BLEService
{
	const static uint16_t SERVICE_BATTERY_UUID						 = GattService::UUID_BATTERY_SERVICE;
	const static uint16_t BATTERY_LEVEL_WRITABLE_CHARACTERISTIC_UUID = GattCharacteristic::UUID_BATTERY_LEVEL_CHAR;

	const static uint8_t CHARACTERISTICS_COUNT = 1;

  public:
	BLEServiceBattery(rtos::EventFlags &event_flags)
		: interface::BLEService(SERVICE_BATTERY_UUID, _characteristic_table.data(), CHARACTERISTICS_COUNT) {};

	void setBatteryLevel(uint8_t value)
	{
		if (value <= 100) {
			_battery_level_characteristic_value = value;
		}
	}

	void onDataWritten(const GattWriteCallbackParams &params) final {};
	void updateData(updateServiceFunction &update) final
	{
		update(_battery_level_writable_characteristic.getValueHandle(), &_battery_level_characteristic_value, 1);
	};

  private:
	uint8_t _battery_level_characteristic_value {};
	ReadOnlyGattCharacteristic<uint8_t> _battery_level_writable_characteristic {
		BATTERY_LEVEL_WRITABLE_CHARACTERISTIC_UUID, &_battery_level_characteristic_value,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

	std::array<GattCharacteristic *, CHARACTERISTICS_COUNT> _characteristic_table {
		&_battery_level_writable_characteristic};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_SERVICE_BATTERY_H_
