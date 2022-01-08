// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_SERVICE_MONITORING_H_
#define _LEKA_OS_LIB_BLE_SERVICE_MONITORING_H_

#include "rtos/EventFlags.h"

#include "Flags.h"
#include "interface/drivers/BLEService.h"

namespace leka {

class BLEServiceMonitoring : public interface::BLEService
{
	const static uint16_t SERVICE_MONITORING_UUID = 0xA000;

	const static uint16_t PING_WRITABLE_CHARACTERISTIC_UUID				  = 0x0001;
	const static uint16_t REBOOT_INSTRUCTION_WRITABLE_CHARACTERISTIC_UUID = 0x0002;
	const static uint16_t MODE_WRITABLE_CHARACTERISTIC_UUID				  = 0x0003;

	const static uint16_t WIFI_SSID_WRITABLE_CHARACTERISTIC_UUID = 0x1001;
	const static uint16_t WIFI_PASS_WRITABLE_CHARACTERISTIC_UUID = 0x1002;

	const static uint8_t CHARACTERISTICS_COUNT = 5;

  public:
	BLEServiceMonitoring(rtos::EventFlags &event_flags)
		: interface::BLEService(SERVICE_MONITORING_UUID, _characteristic_table.data(), CHARACTERISTICS_COUNT),
		  _event_flags(event_flags) {};

	auto getPing() const -> GattAttribute::Handle_t { return _ping_characteristic_value; };
	auto getRebootInstruction() const -> GattAttribute::Handle_t { return _reboot_instruction_characteristic_value; };
	auto getMode() const -> GattAttribute::Handle_t { return _mode_characteristic_value; };
	auto getWifiSSID() const -> std::string { return _wifi_ssid_characteristic_value.data(); };
	auto getWifiPass() const -> std::string { return _wifi_pass_characteristic_value.data(); };

	void onDataWritten(const GattWriteCallbackParams &params) final
	{
		if (params.handle == _ping_writable_characteristic.getValueHandle()) {
			std::fill_n(&_ping_characteristic_value, 1, '\0');
			std::copy(params.data, params.data + 1, &_ping_characteristic_value);
			_event_flags.set(BLE_PING_FLAG);
		} else if (params.handle == _reboot_instruction_writable_characteristic.getValueHandle()) {
			std::fill_n(&_reboot_instruction_characteristic_value, 1, '\0');
			std::copy(params.data, params.data + 1, &_reboot_instruction_characteristic_value);
			_event_flags.set(BLE_REBOOT_INSTRUCTION_FLAG);
		} else if (params.handle == _mode_writable_characteristic.getValueHandle()) {
			std::fill_n(&_mode_characteristic_value, 1, '\0');
			std::copy(params.data, params.data + 1, &_mode_characteristic_value);
			_event_flags.set(BLE_MODE_FLAG);
		} else if (params.handle == _wifi_ssid_writable_characteristic.getValueHandle()) {
			if (params.offset == 0) {
				std::fill_n(_wifi_ssid_characteristic_value.begin(), std::size(_wifi_ssid_characteristic_value), '\0');
			}
			std::copy(params.data, params.data + params.len, _wifi_ssid_characteristic_value.begin() + params.offset);
			// log_info("Data received: length: %d, offset: %d, data: \n%s", params.len, params.offset,
			// 		 _wifi_ssid_characteristic_value.data());
			_event_flags.set(WIFI_SSID);
		} else if (params.handle == _wifi_pass_writable_characteristic.getValueHandle()) {
			if (params.offset == 0) {
				std::fill_n(_wifi_pass_characteristic_value.begin(), std::size(_wifi_pass_characteristic_value), '\0');
			}
			std::copy(params.data, params.data + params.len, _wifi_pass_characteristic_value.begin() + params.offset);
			// log_info("Data received: length: %d, offset: %d, data: \n%s", params.len, params.offset,
			// 		 _wifi_pass_characteristic_value.data());
			_event_flags.set(WIFI_PASS);
		}
		_event_flags.set(NEW_BLE_MESSAGE_FLAG);
	};
	void updateData(updateServiceFunction &update) final {};

  private:
	rtos::EventFlags &_event_flags;

	bool _reboot_instruction_characteristic_value {};
	WriteOnlyGattCharacteristic<bool> _reboot_instruction_writable_characteristic {
		REBOOT_INSTRUCTION_WRITABLE_CHARACTERISTIC_UUID, &_reboot_instruction_characteristic_value};

	bool _ping_characteristic_value {};
	WriteOnlyGattCharacteristic<bool> _ping_writable_characteristic {PING_WRITABLE_CHARACTERISTIC_UUID,
																	 &_ping_characteristic_value};

	uint8_t _mode_characteristic_value {};
	WriteOnlyGattCharacteristic<uint8_t> _mode_writable_characteristic {MODE_WRITABLE_CHARACTERISTIC_UUID,
																		&_mode_characteristic_value};

	std::array<char, 100> _wifi_ssid_characteristic_value {};
	WriteOnlyArrayGattCharacteristic<char, 100> _wifi_ssid_writable_characteristic {
		WIFI_SSID_WRITABLE_CHARACTERISTIC_UUID, _wifi_ssid_characteristic_value.begin()};

	std::array<char, 100> _wifi_pass_characteristic_value {};
	WriteOnlyArrayGattCharacteristic<char, 100> _wifi_pass_writable_characteristic {
		WIFI_PASS_WRITABLE_CHARACTERISTIC_UUID, _wifi_pass_characteristic_value.begin()};

	std::array<GattCharacteristic *, CHARACTERISTICS_COUNT> _characteristic_table {
		&_ping_writable_characteristic, &_reboot_instruction_writable_characteristic, &_mode_writable_characteristic,
		&_wifi_ssid_writable_characteristic, &_wifi_pass_writable_characteristic};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_SERVICE_MONITORING_H_
