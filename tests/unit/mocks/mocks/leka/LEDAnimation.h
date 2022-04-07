// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LedKit.h"
#include "gmock/gmock.h"

namespace leka::mock {

class LEDAnimation : public interface::LEDAnimation
{
  public:
	MOCK_METHOD(void, start, (), (override));
	MOCK_METHOD(void, stop, (), (override));
	MOCK_METHOD(void, run, (), (override));

	MOCK_METHOD(void, setLeds, (interface::LED &, interface::LED &), (override));
	MOCK_METHOD(bool, isRunning, (), (override));
};

}	// namespace leka::mock
