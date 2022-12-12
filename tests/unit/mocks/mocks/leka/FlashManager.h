// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/FlashManager.h"

namespace leka::mock {

class FlashManager : public interface::FlashManager
{
  public:
	FlashManager();
	virtual ~FlashManager();

	MOCK_METHOD(uint8_t, getStatusRegister, (), (override));

	MOCK_METHOD(void, waitForChipAvailable, (), (override));
	MOCK_METHOD(bool, chipIsNotAvailable, (), (override));

	MOCK_METHOD(void, enableWrite, (), (override));
	MOCK_METHOD(bool, writeIsNotEnabled, (), (override));

	MOCK_METHOD(void, reset, (), (override));
	MOCK_METHOD(void, erase, (), (override));
};

}	// namespace leka::mock
