// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/libs/LedKit.h"

namespace leka::mock {

class LedKit : public interface::LedKit
{
  public:
	MOCK_METHOD(void, init, (), (override));
	MOCK_METHOD(void, start, (interface::LEDAnimation * animation), (override));
	MOCK_METHOD(void, run, (), (override));
	MOCK_METHOD(void, stop, (), (override));
};

}	// namespace leka::mock
