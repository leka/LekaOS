// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/Behavior.h"

namespace leka::mock {

class Behavior : public interface::Behavior
{
  public:
	MOCK_METHOD(BehaviorID, id, (), (override));

	MOCK_METHOD(void, run, (), (override));
	MOCK_METHOD(void, stop, (), (override));
};

}	// namespace leka::mock
