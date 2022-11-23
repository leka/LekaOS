// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <tuple>

#include "tests/config.h"

using namespace boost::ut;

// ? Example taken from:
// ? https://github.com/boost-ext/ut/blob/master/example/parameterized.cpp

suite suite_parameterized = [] {
	for (auto i: std::vector {1, 2, 3}) {
		test("parametrized: for loop / val = " + std::to_string(i)) = [i] { expect(that % i > 0); };
	}

	"parametrized: | operator + std::vector"_test = [](auto arg) { expect(arg > 0_i) << "all values greater than 0"; } |
													std::vector {1, 2, 3};

	"parametrized: | operator + std::tuple"_test = []<typename T>() mutable {
		expect(std::is_integral_v<T>) << "all types are integrals";
	} | std::tuple<bool, int> {};

	std::apply(
		[]([[maybe_unused]] auto... args) {
			((test("parametrized: std::apply / val = " + std::to_string(args)) =
				  [&args] {
					  using TArgs = decltype(args);
					  expect(std::is_integral_v<TArgs> >> fatal);
					  expect(42_i == static_cast<int>(args) or args);
					  expect(type<TArgs> == type<int> or type<TArgs> == type<bool>);
				  }),
			 ...);
		},
		std::tuple {42, true});

	"parametrized: template<>"_test = []<class TArg>(TArg arg) {
		expect(std::is_integral_v<TArg> >> fatal);
		expect(42_i == static_cast<int>(arg) or arg);
		expect(type<TArg> == type<int> or type<TArg> == type<bool>);
	} | std::tuple {42, true};
};
