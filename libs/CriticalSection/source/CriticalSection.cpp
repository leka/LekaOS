// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CriticalSection.h"

#include "platform/mbed_critical.h"

using namespace leka;

void CriticalSection::lock()
{
	core_util_critical_section_enter();
}

void CriticalSection::unlock()
{
	core_util_critical_section_exit();
}
