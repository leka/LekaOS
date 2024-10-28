// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/DeepSleepEnabled.h"

namespace leka::mock {

class DeepSleepEnabled : public interface::DeepSleepEnabled
{
  public:
	MOCK_METHOD(void, enableDeepSleep, (), (override));
	MOCK_METHOD(void, disableDeepSleep, (), (override));
};

}	// namespace leka::mock
