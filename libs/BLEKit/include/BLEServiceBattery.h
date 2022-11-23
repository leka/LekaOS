// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <span>
#include <tuple>

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceBattery : public interface::BLEService
{
  public:
	BLEServiceBattery() : interface::BLEService(service::battery::uuid, _characteristic_table) {}

	void setBatteryLevel(uint8_t value)
	{
		if (value <= 100) {
			_battery_level_characteristic_value = value;
		} else {
			_battery_level_characteristic_value = 100;
		}
		auto data = std::make_tuple(_battery_level_writable_characteristic.getValueHandle(),
									std::span(&_battery_level_characteristic_value, 1));
		sendData(data);
	}

	void onDataReceived(const data_received_handle_t &params) final
	{
		// do nothing
	}

	void onDataRequested(const data_requested_handle_t &params) final
	{
		// do nothing
	}

  private:
	data_to_send_handle_t send_data_function {};

	uint8_t _battery_level_characteristic_value {};
	ReadOnlyGattCharacteristic<uint8_t> _battery_level_writable_characteristic {
		service::battery::characteristic::level, &_battery_level_characteristic_value,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

	std::array<GattCharacteristic *, 1> _characteristic_table {&_battery_level_writable_characteristic};
};

}	// namespace leka
