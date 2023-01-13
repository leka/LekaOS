// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "tests/config.h"

using namespace boost::ut;

// ? Example taken from:
// ? https://github.com/boost-ext/ut/blob/master/example/mut.cpp

suite suite_mutable = [] {
	auto i = 0;

	"mutable"_test = [i] {
		expect((i == 0_i) >> fatal);

		should("++") = [i] {
			expect(++mut(i) == 1_i);
			expect(i == 1_i);
		};

		should("--") = [i] {
			expect(--mut(i) == -1_i);
			expect(i == -1_i);
		};
	};
};
