// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_BLE_SERVICE_EXAMPLE_H_
#define _LEKA_OS_LIB_BLE_SERVICE_EXAMPLE_H_

#include "LogKit.h"
#include "interface/drivers/BLEService.h"

namespace leka {

class BLEServiceExample : public interface::BLEService
{
	const static uint16_t SERVICE_UUID				   = 0xA000;
	const static uint16_t WRITABLE_CHARACTERISTIC_UUID = 0xA001;

	const static uint8_t CHARACTERITICS_COUNT = 1;

  public:
	BLEServiceExample() : interface::BLEService(SERVICE_UUID, _characteristic_table.data(), CHARACTERITICS_COUNT) {};

	void onDataWritten(const GattWriteCallbackParams &params) final
	{
		if (params.offset == 0) {
			std::fill_n(_characteristic_value.begin(), std::size(_characteristic_value), '\0');
		}
		std::copy(params.data, params.data + params.len, _characteristic_value.begin() + params.offset);
		log_info("Data received: length: %d, offset: %d, data: \n%s", params.len, params.offset,
				 _characteristic_value.data());
	};
	void updateData(updateServiceFunction &update) final {};

  private:
	std::array<char, 100> _characteristic_value {};
	WriteOnlyArrayGattCharacteristic<char, 100> _writable_characteristic {WRITABLE_CHARACTERISTIC_UUID,
																		  _characteristic_value.begin()};

	std::array<GattCharacteristic *, CHARACTERITICS_COUNT> _characteristic_table {&_writable_characteristic};
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_BLE_SERVICE_EXAMPLE_H_
