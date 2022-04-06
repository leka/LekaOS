// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreEventFlags.h"

using namespace leka;

auto CoreEventFlags::wait_any(eventflags_t flag) -> void
{
	_event_flags.wait_any(flag);
}

auto CoreEventFlags::set(eventflags_t flags) -> std::optional<eventflags_t>
{
	if (_event_flags.set(flags) == osFlagsError) {
		return {};
	}
	return flags;
}

auto CoreEventFlags::clear(eventflags_t flags) -> std::optional<eventflags_t>
{
	if (_event_flags.clear(flags) == osFlagsError) {
		return {};
	}
	return flags;
}

auto CoreEventFlags::get() const -> std::optional<eventflags_t>
{
	auto flags = _event_flags.get();
	if (flags == 0U) {
		return {};
	}
	return flags;
}
