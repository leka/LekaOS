// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <memory>

#include "tests/config.h"

using namespace boost::ut;

// ? Example taken from:
// ? https://github.com/boost-ext/ut/blob/master/example/expect.cpp

inline constexpr auto sum = [](auto... args) { return (0 + ... + args); };

suite suite_expect = [] {
	"operators: == / != / >= / <="_test = [] {
		expect(0_i == sum());
		expect(2_i != sum(1, 2));
		expect(sum(1) >= 0_i);
		expect(sum(1) <= 1_i);
	};

	"expressions: and / or / mix "_test = [] {
		expect(100_i == sum() and 42_i == sum(40, 2));
		expect(100_i == sum() or 0_i == sum());
		expect(2_i == sum() or (sum() != 0_i or sum(1) > 0_i)) << "compound";
	};

	"output message after test"_test = [] { expect(3_i == sum(1, 2)) << "wrong sum"; };

	"expect that... "_test = [] {
		expect(that % 0 == sum());
		expect(that % 43 == sum(40, 2) and that % (1 + 2) == sum(1, 2));
		expect(that % 1 != 2 or 2_i > 3);
	};

	"functions: eq / neq / gt / ge / lt / le"_test = [] {
		expect(eq(42, sum(40, 2)));
		expect(neq(2, 2));
		expect(eq(sum(1), 1) and neq(sum(1, 2), 2));
		expect(eq(1, 1) and that % 1 == 1 and 1_i == 1);
		expect(gt(2, 1));
		expect(ge(2, 1) and ge(1, 1) and ge(2, 2));
		expect(lt(1, -2));
		expect(le(1, 2) and le(1, 1) and le(2, 2));
	};

	"floating points w/ epsilon"_test = [] {
		expect(42.1_d == 42.101) << "epsilon=0.1";
		expect(42.10_d == 42.101) << "epsilon=0.01";
		expect(42.10000001 == 42.1_d) << "epsilon=0.1";
	};

	"std::string/string_view literals s/sv"_test = [] {
		using namespace std::literals::string_view_literals;
		using namespace std::literals::string_literals;

		expect("str"s == "str"s);
		expect("str1"s != "str2"s);

		expect("str"sv == "str"sv);
		expect("str1"sv != "str2"sv);

		expect("str"sv == "str"s);
		expect("str1"sv != "str2"s);
		expect("str"s == "str"sv);
		expect("str1"s != "str2"sv);
	};

	"type<> comparision"_test = [] {
		expect(type<int> != type<long>);
		expect(type<float> != type<double>);

		[[maybe_unused]] const auto i = 0;
		expect(type<const int> == type<decltype(i)>);
		expect(type<decltype(i)> != type<int>);
	};

	"containers: std::vector / std::array"_test = [] {
		std::vector v1 {1, 5, 3};
		std::vector v2 {1, 2, 3};
		expect(v1 == v2);
		expect(std::vector {"a", "b"} != std::vector {"c"});
		expect(std::array {true, false} != std::array {false, true});
	};

	"compiletime/runtime constants"_test = [] {
		constexpr auto compile_time_v = 42;
		auto run_time_v				  = 99;
		expect(constant<42_i == compile_time_v> and run_time_v == 99_i);
	};

	"bool conversion"_test = [] { expect(static_cast<bool>(std::make_unique<int>())); };

	"boolean literals"_test = [] {
		expect("true"_b);
		expect("true"_b or not "true"_b);
		expect((not "true"_b) != "true"_b);
		expect("has value"_b == "value is set"_b);
	};
};
