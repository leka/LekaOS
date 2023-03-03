// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/Activity.h"

namespace leka::mock {

class Activity : public interface::Activity
{
  public:
	MOCK_METHOD(void, start, (const std::function<void()> &), ());
	MOCK_METHOD(void, stop, (), ());
};

}	// namespace leka::mock
