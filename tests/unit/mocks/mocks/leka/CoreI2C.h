// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreI2C.h"
#include "gmock/gmock.h"

namespace leka::mock {

class CoreI2C : public interface::I2C
{
  public:
	MOCK_METHOD(int, read, (int, uint8_t *, int, bool), (override));
	MOCK_METHOD(int, write, (int, const uint8_t *, int, bool), (override));
};

}	// namespace leka::mock
