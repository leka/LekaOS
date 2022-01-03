// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_EVENTS_STATE_MACHINE_
#define _LEKA_OS_EVENTS_STATE_MACHINE_

#include <cstdint>

namespace leka {

enum class Events : uint32_t
{
	NONE	= 0,
	WAKEUP	= (1UL << 0),
	TIMEOUT = (1UL << 1),
	ALL		= WAKEUP + TIMEOUT
};

}	// namespace leka

#endif	 // _LEKA_OS_EVENTS_STATE_MACHINE_
