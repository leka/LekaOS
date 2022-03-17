// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LogKit.h"
#include "boost/sml.hpp"

namespace leka::system::robot::sm {

struct logger {
	// ? prefix_size is used to remove the long leka::system::robot:: namespace from the output
	static constexpr auto prefix_size = std::size(std::string_view {"leka::system::robot::"});

	template <typename T>
	auto name()
	{
		return boost::sml::aux::get_type_name<T>() + prefix_size;
	}

	template <typename SM, typename TEvent>
	void log_process_event(const TEvent &event)
	{
		log_debug("%s", name<TEvent>());
	}

	template <typename SM, typename TGuard, typename TEvent>
	void log_guard(const TGuard &guard, const TEvent &event, bool result)
	{
		log_debug("%s [%s == %s]", name<TEvent>(), name<TGuard>(), (result ? "(true)" : "(false)"));
	}

	template <typename SM, typename TAction, typename TEvent>
	void log_action(const TAction &action, const TEvent &event)
	{
		log_debug("%s", name<TAction>());
	}

	template <typename SM, typename TSrcState, typename TDstState>
	void log_state_change(const TSrcState &src, const TDstState &dst)
	{
		log_debug("%s -> %s", src.c_str() + prefix_size, dst.c_str() + prefix_size);
	}
};

}	// namespace leka::system::robot::sm
