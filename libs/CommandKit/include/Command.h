// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstddef>
#include <cstdint>

namespace leka {

struct Command {
	virtual ~Command() = default;

	virtual auto id() -> uint8_t = 0;

	virtual auto data() -> uint8_t * = 0;

	[[nodiscard]] virtual auto size() const -> std::size_t = 0;

	virtual void operator()() = 0;
};

}	// namespace leka
