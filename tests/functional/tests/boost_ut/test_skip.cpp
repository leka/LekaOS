// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "tests/config.h"

using namespace boost::ut;

// ? Example taken from:
// ? https://github.com/boost-ext/ut/blob/master/example/skip.cpp

suite suite_skip = [] {
	skip / "1st skipped test"_test = [] {
		expect(42_i == 43) << "should not fire!";
		expect(false) << "should fail!";
	};

	skip / test("2nd skipped test") = [] {
		expect(42_i == 43) << "should not fire!";
		expect(false) << "should fail!";
	};
};
