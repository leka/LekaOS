// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "LogKit.h"
#include "boost/sml.hpp"

namespace leka::system::robot::sm {

struct logger {
	auto trim_namespaces(const char *str) const
	{
		auto sstr = std::string {str};

		static const auto namespaces = std::to_array<const std::string>({
			std::string {"leka::system::robot::"},
			std::string {"boost::sml::back::"},
			std::string {"boost::ext::sml::v1_1_4::back::"},
		});

		for (const auto &nmspc: namespaces) {
			auto pos = std::string::npos;
			while ((pos = sstr.find(nmspc)) != std::string::npos) {
				sstr.erase(pos, nmspc.length());
			}
		}

		return sstr;
	}

	template <typename T>
	auto name()
	{
		auto name = trim_namespaces(boost::sml::aux::get_type_name<T>());
		return name;
	}

	template <typename SM, typename TEvent>
	void log_process_event([[maybe_unused]] const TEvent &event)
	{
		auto e = name<TEvent>();
		log_info("%s", e.c_str());
	}

	template <typename SM, typename TGuard, typename TEvent>
	void log_guard([[maybe_unused]] const TGuard &guard, [[maybe_unused]] const TEvent &event, bool result)
	{
		auto e = name<TEvent>();
		auto g = name<TGuard>();
		log_info("%s [%s == %s]", e.c_str(), g.c_str(), (result ? "true" : "false"));
	}

	template <typename SM, typename TAction, typename TEvent>
	void log_action([[maybe_unused]] const TAction &action, [[maybe_unused]] const TEvent &event)
	{
		auto a = name<TAction>();
		log_info("%s", a.c_str());
	}

	template <typename SM, typename TSrcState, typename TDstState>
	void log_state_change(const TSrcState &src, const TDstState &dst) const
	{
		auto s = trim_namespaces(src.c_str());
		auto d = trim_namespaces(dst.c_str());
		log_info("%s -> %s", s.c_str(), d.c_str());
	}
};

}	// namespace leka::system::robot::sm
