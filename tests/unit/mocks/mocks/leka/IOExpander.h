// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/IOExpander.h"

namespace leka::mock {

template <typename pin_underlying_type_t>
class IOExpander : public interface::IOExpander<pin_underlying_type_t>
{
  public:
	// IOExpander();
	// virtual ~IOExpander();

	MOCK_METHOD(void, setPinAsInput, (pin_underlying_type_t), (override));
	MOCK_METHOD(void, setPinAsOutput, (pin_underlying_type_t), (override));

	MOCK_METHOD(void, setModeForPin, (pin_underlying_type_t, PinMode), ());
	MOCK_METHOD(PinMode, getModeForPin, (pin_underlying_type_t), (override));

	MOCK_METHOD(int, readPin, (pin_underlying_type_t), (override));
	MOCK_METHOD(void, writePin, (pin_underlying_type_t, int), (override));
};

}	// namespace leka::mock
