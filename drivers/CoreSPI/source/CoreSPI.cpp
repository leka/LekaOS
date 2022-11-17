// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreSPI.h"

using namespace leka;

auto CoreSPI::write(std::span<const uint8_t> data) -> std::size_t
{
	// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
	return _spi.write(reinterpret_cast<const char *>(data.data()), std::size(data), nullptr, 0);
}
