// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/PwmOut.h"

namespace leka::mock {

class PwmOut : public interface::PwmOut
{
  public:
	MOCK_METHOD(float, read, (), (override));
	MOCK_METHOD(void, write, (float), (override));
	MOCK_METHOD(void, period, (float), (override));
	MOCK_METHOD(void, suspend, (), (override));
	MOCK_METHOD(void, resume, (), (override));
};

}	// namespace leka::mock
