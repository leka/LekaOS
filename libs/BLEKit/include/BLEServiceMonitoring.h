// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <functional>
#include <span>
#include <tuple>

#include "platform/mbed_power_mgmt.h"

#include "MemoryUtils.h"
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

	void setNegotiatedMtu(uint16_t value)
	{
		_negotiated_mtu = value;

		auto _high_byte = utils::memory::getHighByte(_negotiated_mtu);
		auto _low_byte	= utils::memory::getLowByte(_negotiated_mtu);

		auto data =
			std::make_tuple(_negotiated_mtu_characteristic.getValueHandle(), std::to_array({_high_byte, _low_byte}));

		sendData(data);
	}

	void setTemperature(float value)
	{
		std::memcpy(temperature.data(), &value, 4);

		auto data = std::make_tuple(_temperature_characteristic.getValueHandle(), temperature);
		sendData(data);
	}

	void setHumidity(float value)
	{
		std::memcpy(humidity.data(), &value, 4);

		auto data = std::make_tuple(_humidity_characteristic.getValueHandle(), humidity);
		sendData(data);
	}

	auto isScreensaverEnable() const -> bool { return _screensaver_enable; }

	void onTemperatureRequested(const std::function<void()> &callback)
	{
		_on_temperature_requested_callback = callback;
	}

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
		if (params.handle == _temperature_characteristic.getValueHandle() &&
			_on_temperature_requested_callback != nullptr) {
			_on_temperature_requested_callback();
		}
	}

  private:
	uint8_t _charging_status {0x00};
	ReadOnlyGattCharacteristic<uint8_t> _charging_status_characteristic {
		service::monitoring::characteristic::charging_status,
		&_charging_status,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY,
	};

	std::array<uint8_t, 4> temperature {};
	ReadOnlyArrayGattCharacteristic<uint8_t, 4> _temperature_characteristic {
		service::monitoring::characteristic::temperature, temperature.begin(),
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};
	std::function<void()> _on_temperature_requested_callback {};

	std::array<uint8_t, 4> humidity {};
	ReadOnlyArrayGattCharacteristic<uint8_t, 4> _humidity_characteristic {
		service::monitoring::characteristic::humidity, humidity.begin(),
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY};

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

	uint16_t _negotiated_mtu {0x00};
	ReadOnlyGattCharacteristic<uint16_t> _negotiated_mtu_characteristic {
		service::monitoring::characteristic::negotiated_mtu,
		&_negotiated_mtu,
		GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY,
	};

	std::array<GattCharacteristic *, 7> _characteristic_table {
		&_charging_status_characteristic, &_screensaver_enable_characteristic, &_soft_reboot_characteristic,
		&_hard_reboot_characteristic,	  &_negotiated_mtu_characteristic,	   &_temperature_characteristic,
		&_humidity_characteristic,
	};
};

}	// namespace leka
