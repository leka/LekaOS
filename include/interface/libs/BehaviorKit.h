// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "interface/Behavior.h"
#include "internal/BehaviorID.h"

namespace leka::interface {

class BehaviorKit
{
  public:
	virtual ~BehaviorKit() = default;

	virtual void registerBehaviors(std::span<interface::Behavior *> behaviors) = 0;
	virtual void start(interface::Behavior *behavior)						   = 0;
	virtual void start(BehaviorID id)										   = 0;
	virtual void stop()														   = 0;
};

}	// namespace leka::interface