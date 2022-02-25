// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/Ticker.h"

namespace leka::mock {

class Ticker : public interface::Ticker
{
  public:
	MOCK_METHOD(void, onTick, (callback_t const &), (override));

	MOCK_METHOD(void, start, (std::chrono::microseconds), (override));
	MOCK_METHOD(void, stop, (), (override));
};

}	// namespace leka::mock
