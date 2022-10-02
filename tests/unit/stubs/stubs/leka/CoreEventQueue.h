// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "include/CoreEventQueue.h"

namespace leka {

extern bool spy_CoreEventQueue_did_call_function;

extern std::function<void()> spy_CoreEventQueue_on_dispatch_forever_call;

}	// namespace leka
