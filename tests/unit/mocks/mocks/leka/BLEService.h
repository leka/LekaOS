// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_BLE_SERVICE_MOCK_H_
#define _LEKA_OS_BLE_SERVICE_MOCK_H_

#include "gmock/gmock.h"
#include "internal/BLEService.h"

namespace leka::mock {

class BLEService : public interface::BLEService
{
  public:
	BLEService(const UUID &uuid, std::span<GattCharacteristic *> characteristics)
		: interface::BLEService(uuid, characteristics)
	{
	}

	MOCK_METHOD(void, onDataWritten, (const GattWriteCallbackParams &params), (override));

	MOCK_METHOD(void, registerUpdateDataFunction, (update_data_function_t const &function), (override));
	MOCK_METHOD(void, updateData, (), (override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_BLE_SERVICE_MOCK_H_
