// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace leka::interface {

class EventFlags
{
  public:
	using eventflags_t	  = uint32_t;
	virtual ~EventFlags() = default;

	virtual auto wait_any(eventflags_t flag, bool clear = true) -> void	 = 0;
	virtual auto set(eventflags_t flag) -> std::optional<eventflags_t>	 = 0;
	virtual auto clear(eventflags_t flag) -> std::optional<eventflags_t> = 0;

	[[nodiscard]] virtual auto get() const -> eventflags_t = 0;
};

}	// namespace leka::interface
