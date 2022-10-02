// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <optional>

#include "rtos/EventFlags.h"

#include "interface/drivers/EventFlags.h"

namespace leka {

class CoreEventFlags : public interface::EventFlags
{
  public:
	explicit CoreEventFlags() = default;

	auto wait_any(eventflags_t flag, bool clear = true) -> void final;

	auto set(eventflags_t flag) -> std::optional<eventflags_t> final;
	auto clear(eventflags_t flag) -> std::optional<eventflags_t> final;

	[[nodiscard]] auto get() const -> eventflags_t final;

  private:
	rtos::EventFlags _event_flags {};
};

}	// namespace leka
