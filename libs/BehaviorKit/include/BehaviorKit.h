// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "interface/Behavior.h"
#include "interface/libs/BehaviorKit.h"
#include "internal/BehaviorID.h"

namespace leka {

class BehaviorKit : public interface::BehaviorKit
{
  public:
	explicit BehaviorKit() = default;

	void registerBehaviors(std::span<interface::Behavior *> behaviors) final;

	void start(interface::Behavior *behavior) final;
	void start(BehaviorID id) final;
	void stop() final;

  private:
	std::span<interface::Behavior *> _behaviors {};
	interface::Behavior *_behavior = nullptr;
};

}	// namespace leka
