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

	MOCK_METHOD(void, onDataReceived, (const data_received_handle_t &params), (override));

	MOCK_METHOD(void, onDataReadyToSend, (const data_to_send_handler_t &function), (override));
	MOCK_METHOD(void, sendData, (), (override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_BLE_SERVICE_MOCK_H_
