// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVERS_PWMOUT_H_
#define _LEKA_OS_DRIVERS_PWMOUT_H_

#include "gmock/gmock.h"
#include "interface/drivers/PwmOut.h"

namespace leka {

class PwmOutMock : public interface::PwmOut
{
  public:
	MOCK_METHOD(float, read, (), (override));
	MOCK_METHOD(void, write, (float), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_DRIVERS_PWMOUT_H_
