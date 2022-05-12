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
	MOCK_METHOD(void, setPinAsInput, (pin_underlying_type_t), (override));
	MOCK_METHOD(int, readInputPin, (pin_underlying_type_t), (override));

	MOCK_METHOD(void, setModeForInputPin, (pin_underlying_type_t, PinMode), ());
	MOCK_METHOD(PinMode, getModeForInputPin, (pin_underlying_type_t), (override));

	MOCK_METHOD(void, setPinAsOutput, (pin_underlying_type_t), (override));
	MOCK_METHOD(int, readOutputPin, (pin_underlying_type_t), (override));
	MOCK_METHOD(void, writeOutputPin, (pin_underlying_type_t, int), (override));
};

}	// namespace leka::mock
