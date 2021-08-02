// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDACTimer.h"

namespace leka {

CoreDACTimer::CoreDACTimer(LKCoreSTM32HalBase &hal) : _hal(hal)
{
	_htim.Instance = TIM6;
}

}	// namespace leka