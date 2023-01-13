// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "tests/config.h"

using namespace boost::ut;

// ? Example taken from:
// ? https://github.com/boost-ext/ut/blob/master/example/should.cpp

suite suite_should = [] {
	auto i = 0;
	expect((i == 0_i) >> fatal);

	should("return increased number for ++") = [i] { expect(++mut(i) == 1_i); };
	should("return decreased number for --") = [i]() mutable { expect(--i == -1_i); };
};
