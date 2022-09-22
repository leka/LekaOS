// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

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
	MOCK_METHOD(void, onDataRequested, (const data_requested_handle_t &params), (override));
};

}	// namespace leka::mock
