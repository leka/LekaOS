// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <functional>
#include <span>
#include <tuple>

#include "platform/mbed_power_mgmt.h"

#include "internal/BLEService.h"
#include "internal/ServicesCharacteristics.h"

namespace leka {

class BLEServiceMonitoring : public interface::BLEService
{
  public:
	BLEServiceMonitoring() : interface::BLEService(service::monitoring::uuid, _characteristic_table) {}

	void setChargingStatus(bool value)
	{
		charging_status = static_cast<uint8_t>(value);

		auto data = std::make_tuple(_charging_status_characteristic.getValueHandle(), std::span(&charging_status, 1));
		sendData(data);
	}

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == soft_reboot_characteristic.getValueHandle()) {
			soft_reboot = static_cast<bool>(params.data[0]);
			if (soft_reboot && _on_soft_reboot) {
				_on_soft_reboot();
			}
		}
		if (params.handle == hard_reboot_characteristic.getValueHandle()) {
			hard_reboot = static_cast<bool>(params.data[0]);
			if (hard_reboot) {
				system_reset();
			}
		}
	}

	void onSoftReboot(const std::function<void()> &callback) { _on_soft_reboot = callback; }

	void onDataRequested(const data_requested_handle_t &params) final
	{
		// do nothing
	}

  private:
	uint8_t charging_status {0x00};
	ReadOnlyGattCharacteristic<uint8_t> _charging_status_characteristic {
		service::monitoring::characteristic::charging_status, &charging_status,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

	bool soft_reboot {false};
	WriteOnlyGattCharacteristic<bool> soft_reboot_characteristic {service::monitoring::characteristic::soft_reboot,
																  &soft_reboot};
	std::function<void()> _on_soft_reboot {};

	bool hard_reboot {false};
	WriteOnlyGattCharacteristic<bool> hard_reboot_characteristic {service::monitoring::characteristic::hard_reboot,
																  &hard_reboot};

	std::array<GattCharacteristic *, 3> _characteristic_table {
		&_charging_status_characteristic, &soft_reboot_characteristic, &hard_reboot_characteristic};
};

}	// namespace leka
