// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLL.h"

using namespace leka;

void CoreLL::rawMemoryWrite(uintptr_t destination, uint32_t data)
{
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	auto *ptr = reinterpret_cast<uint32_t *>(destination);
	*ptr	  = data;
}

void CoreLL::rawMemoryWrite(uintptr_t destination, uint8_t data)
{
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	auto *ptr = reinterpret_cast<uint8_t *>(destination);
	*ptr	  = data;
}
