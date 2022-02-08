// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_BLE_SERVICE_MOCK_H_
#define _LEKA_OS_BLE_SERVICE_MOCK_H_

#include "gmock/gmock.h"
#include "interface/drivers/BLEService.h"

namespace leka::mock {

class BLEService : public interface::BLEService
{
  public:
	BLEService(const UUID &uuid, std::span<GattCharacteristic *> characteristics, unsigned characteristics_count)
		: interface::BLEService(uuid, characteristics, characteristics_count)
	{
	}

	MOCK_METHOD(void, onDataWritten, (const GattWriteCallbackParams &params), (override));
	MOCK_METHOD(void, updateData, (updateServiceFunction & update), (override));

	MOCK_METHOD(uint8_t, getCharacteristicCount, (), ());
	MOCK_METHOD(GattCharacteristic *, getCharacteristic, (uint8_t index), ());
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_BLE_SERVICE_MOCK_H_
