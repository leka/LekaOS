
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CGAnimation.h"
#include "gmock/gmock.h"

namespace leka::mock {

class CGAnimation : public interface::CGAnimation
{
  public:
	MOCK_METHOD(void, start, (), (override));
	MOCK_METHOD(void, stop, (), (override));
	MOCK_METHOD(void, run, (), (override));
};

}	// namespace leka::mock
