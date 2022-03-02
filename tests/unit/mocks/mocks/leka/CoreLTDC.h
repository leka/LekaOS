// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/LTDC.hpp"

namespace leka::mock {

class CoreLTDC : public interface::LTDCBase
{
  public:
	MOCK_METHOD(void, initialize, (), (override));
};

}	// namespace leka::mock
