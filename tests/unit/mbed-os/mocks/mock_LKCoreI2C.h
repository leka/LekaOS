// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_I2C_MOCK_H_
#define _LEKA_OS_I2C_MOCK_H_

#include "LKCoreI2C.h"
#include "gmock/gmock.h"

namespace leka {

class LKCoreI2CMock : public interface::LKCoreI2C
{
  public:
	MOCK_METHOD(int, read, (int, char *, int, bool), (override));
	MOCK_METHOD(int, write, (int, const char *, int, bool), (override));
};

}	// namespace leka

#endif	 // _LEKA_OS_I2C_MOCK_H_
