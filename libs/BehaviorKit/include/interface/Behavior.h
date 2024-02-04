// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>
#include <cstdint>

#include "../internal/BehaviorID.h"

namespace leka::interface {

struct Behavior {
	virtual ~Behavior() = default;

	virtual auto id() -> BehaviorID = 0;

	virtual void run()	= 0;
	virtual void stop() = 0;
};

}	// namespace leka::interface
