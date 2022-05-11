// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreEventFlags.h"

using namespace leka;

auto CoreEventFlags::wait_any(eventflags_t flag, bool clear) -> void
{
	_event_flags.wait_any(flag, osWaitForever, clear);
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

auto CoreEventFlags::get() const -> eventflags_t
{
	return _event_flags.get();
}
