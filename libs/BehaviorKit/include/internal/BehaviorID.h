// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>

using BehaviorID = uint16_t;

namespace leka::behavior_id {

inline constexpr BehaviorID none = 0x0000;

inline constexpr BehaviorID autocharge_seal = 0x0100;

}	// namespace leka::behavior_id
