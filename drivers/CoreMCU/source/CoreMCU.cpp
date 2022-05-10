// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMCU.h"

using namespace leka;

// LCOV_EXCL_START

auto CoreMCU::getID() -> MCUID
{
	auto id = MCUID {};

	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	id.front = *reinterpret_cast<uint32_t *>(id_address);
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	id.middle = *reinterpret_cast<uint32_t *>(id_address + 4);
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	id.back = *reinterpret_cast<uint32_t *>(id_address + 8);

	return id;
}

// LCOV_EXCL_STOP
