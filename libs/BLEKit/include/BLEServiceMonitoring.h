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
		_charging_status = static_cast<uint8_t>(value);

		auto data = std::make_tuple(_charging_status_characteristic.getValueHandle(), std::span(&_charging_status, 1));
		sendData(data);
	}

	auto isScreensaverEnable() const -> bool { return _screensaver_enable; }

	void onDataReceived(const data_received_handle_t &params) final
	{
		if (params.handle == _screensaver_enable_characteristic.getValueHandle()) {
			_screensaver_enable = static_cast<bool>(params.data[0]);
		}
		if (params.handle == _soft_reboot_characteristic.getValueHandle()) {
			_soft_reboot = static_cast<bool>(params.data[0]);
			if (_soft_reboot && _on_soft_reboot) {
				_on_soft_reboot();
			}
		}
		if (params.handle == _hard_reboot_characteristic.getValueHandle()) {
			_hard_reboot = static_cast<bool>(params.data[0]);
			if (_hard_reboot) {
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
	uint8_t _charging_status {0x00};
	ReadOnlyGattCharacteristic<uint8_t> _charging_status_characteristic {
		service::monitoring::characteristic::charging_status,
		&_charging_status,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY,
	};

	bool _screensaver_enable {true};
	WriteOnlyGattCharacteristic<bool> _screensaver_enable_characteristic {
		service::monitoring::characteristic::screensaver_enable,
		&_screensaver_enable,
	};

	bool _soft_reboot {false};
	WriteOnlyGattCharacteristic<bool> _soft_reboot_characteristic {
		service::monitoring::characteristic::soft_reboot,
		&_soft_reboot,

	};
	std::function<void()> _on_soft_reboot;

	bool _hard_reboot {false};
	WriteOnlyGattCharacteristic<bool> _hard_reboot_characteristic {
		service::monitoring::characteristic::hard_reboot,
		&_hard_reboot,
	};

	std::array<GattCharacteristic *, 4> _characteristic_table {
		&_charging_status_characteristic,
		&_screensaver_enable_characteristic,
		&_soft_reboot_characteristic,
		&_hard_reboot_characteristic,
	};
};

}	// namespace leka
