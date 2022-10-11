// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./tests/config.h"
#include "boost/ut.hpp"

using namespace boost::ut;

// ? Example taken from:
// ? https://github.com/boost-ext/ut/blob/master/example/spec.cpp

inline constexpr auto sum = [](auto... args) { return (0 + ... + args); };

suite suite_spec = [] {
	using namespace boost::ut::operators::terse;
	using namespace boost::ut::literals;
	using namespace boost::ut::spec;

	describe("sum specs") = [] {
		it("should be 0 for empty params list") = [] { expect(sum() == 0_i); };
		it("should add all args")				= [] { expect(sum(1, 2, 3) == 6_i); };
	};
};
