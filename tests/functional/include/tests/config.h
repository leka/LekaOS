// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "boost/ut.hpp"

namespace ut = boost::ut;

namespace cfg {
struct printer : ut::printer {
	template <class T>
	auto operator<<(T &&t) -> auto &
	{
		std::cout << std::forward<T>(t);
		return *this;
	}
};

}	// namespace cfg

template <>
inline auto ut::cfg<ut::override> = ut::runner<ut::reporter<cfg::printer>> {};
