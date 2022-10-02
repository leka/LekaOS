// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <optional>

#include "gmock/gmock.h"
#include "interface/drivers/EventFlags.h"

namespace mbed::mock {

class EventFlags : public leka::interface::EventFlags
{
  public:
	MOCK_METHOD(void, wait_any, (eventflags_t, bool), (override));
	MOCK_METHOD(std::optional<eventflags_t>, set, (eventflags_t), (override));
	MOCK_METHOD(std::optional<eventflags_t>, clear, (eventflags_t), (override));
	MOCK_METHOD(eventflags_t, get, (), (const, override));
};

}	// namespace mbed::mock
