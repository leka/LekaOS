// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_MOCK_DRIVERS_IO_EXPANDER_H_
#define _LEKA_OS_MOCK_DRIVERS_IO_EXPANDER_H_

#include "gmock/gmock.h"
#include "interface/drivers/IOExpander.h"

namespace leka::mock {

template <typename PinType>
class IOExpander : public interface::IOExpander<PinType>
{
  public:
	MOCK_METHOD(void, setPinAsInput, (PinType), (override));
	MOCK_METHOD(int, readInputPin, (PinType), (override));

	MOCK_METHOD(void, setModeForPin, (PinType, PinMode), ());
	MOCK_METHOD(PinMode, getModeForPin, (PinType), (override));
};

}	// namespace leka::mock

#endif	 // _LEKA_OS_MOCK_DRIVERS_IO_EXPANDER_H_
