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
	const static uint16_t SERVICE_MONITORING_UUID						  = 0xA000;
	const static uint16_t PING_WRITABLE_CHARACTERISTIC_UUID				  = 0xA001;
	const static uint16_t REBOOT_INSTRUCTION_WRITABLE_CHARACTERISTIC_UUID = 0xA002;
	const static uint16_t LEDS_INTENSITY_WRITABLE_CHARACTERISTIC_UUID	  = 0xA003;
	const static uint16_t LCD_INTENSITY_WRITABLE_CHARACTERISTIC_UUID	  = 0xA004;

	const static uint8_t CHARACTERISTICS_COUNT = 4;

  public:
	BLEServiceMonitoring(rtos::EventFlags &event_flags)
		: interface::BLEService(SERVICE_MONITORING_UUID, _characteristic_table.data(), CHARACTERISTICS_COUNT),
		  _event_flags(event_flags) {};

	auto getPing() const -> GattAttribute::Handle_t { return _ping_characteristic_value; };
	auto getRebootInstruction() const -> GattAttribute::Handle_t { return _reboot_instruction_characteristic_value; };
	auto getLedsIntenisty() const -> GattAttribute::Handle_t { return _leds_intensity_characteristic_value; };
	auto getLCDIntensity() const -> GattAttribute::Handle_t { return _lcd_intensity_characteristic_value; };

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
		} else if (params.handle == _leds_intensity_writable_characteristic.getValueHandle()) {
			std::fill_n(&_leds_intensity_characteristic_value, 1, '\0');
			std::copy(params.data, params.data + 1, &_leds_intensity_characteristic_value);
			_event_flags.set(BLE_LEDS_INTENSITY_FLAG);
		} else if (params.handle == _lcd_intensity_writable_characteristic.getValueHandle()) {
			std::fill_n(&_lcd_intensity_characteristic_value, 1, '\0');
			std::copy(params.data, params.data + 1, &_lcd_intensity_characteristic_value);
			_event_flags.set(BLE_LCD_INTENSITY_FLAG);
		}
		_event_flags.set(NEW_BLE_MESSAGE_FLAG);
	};
	void updateData(updateServiceFunction &update) final {};

  private:
	rtos::EventFlags &_event_flags;

	bool _ping_characteristic_value {};
	WriteOnlyGattCharacteristic<bool> _ping_writable_characteristic {PING_WRITABLE_CHARACTERISTIC_UUID,
																	 &_ping_characteristic_value};

	bool _reboot_instruction_characteristic_value {};
	WriteOnlyGattCharacteristic<bool> _reboot_instruction_writable_characteristic {
		REBOOT_INSTRUCTION_WRITABLE_CHARACTERISTIC_UUID, &_reboot_instruction_characteristic_value};

	uint8_t _leds_intensity_characteristic_value {};
	WriteOnlyGattCharacteristic<uint8_t> _leds_intensity_writable_characteristic {
		LEDS_INTENSITY_WRITABLE_CHARACTERISTIC_UUID, &_leds_intensity_characteristic_value};

	uint8_t _lcd_intensity_characteristic_value {};
	WriteOnlyGattCharacteristic<uint8_t> _lcd_intensity_writable_characteristic {
		LCD_INTENSITY_WRITABLE_CHARACTERISTIC_UUID, &_lcd_intensity_characteristic_value};

	std::array<GattCharacteristic *, CHARACTERISTICS_COUNT> _characteristic_table {
		&_ping_writable_characteristic, &_reboot_instruction_writable_characteristic,
		&_leds_intensity_writable_characteristic, &_lcd_intensity_writable_characteristic};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_SERVICE_MONITORING_H_
