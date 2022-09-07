// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include <utility>

#include "rtos/ThisThread.h"

#include "LogKit.h"
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
auto ut::cfg<ut::override> = ut::runner<ut::reporter<cfg::printer>> {};

using namespace leka;
using namespace std::chrono;
using namespace ut;

[[maybe_unused]] constexpr auto sum = [](auto... args) { return (0 + ... + args); };

suite suite_minimal = [] { "run"_test = [] { expect(1 != 2); }; };

suite suite_expect = [] {
	"operators"_test = [] {
		expect(0_i == sum());
		expect(2_i != sum(1, 2));
		expect(sum(1) >= 0_i);
		expect(sum(1) <= 1_i);
	};

	"message"_test = [] { expect(3_i == sum(1, 2)) << "wrong sum"; };

	"expressions"_test = [] {
		expect(0_i == sum() and 42_i == sum(40, 2));
		expect(1_i == sum() or 0_i == sum());
		expect(1_i == sum() or (sum() != 0_i or sum(1) > 0_i)) << "compound";
	};

	"that"_test = [] {
		expect(that % 0 == sum());
		expect(that % 42 == sum(40, 2) and that % (1 + 2) == sum(1, 2));
		expect(that % 1 != 2 or 2_i > 3);
	};

	"eq/neq/gt/ge/lt/le"_test = [] {
		expect(eq(42, sum(40, 2)));
		expect(neq(1, 2));
		expect(eq(sum(1), 1) and neq(sum(1, 2), 2));
		expect(eq(1, 1) and that % 1 == 1 and 1_i == 1);
	};

	"floating points"_test = [] {
		expect(42.1_d == 42.101) << "epsilon=0.1";
		expect(42.10_d == 42.101) << "epsilon=0.01";
		expect(42.10000001 == 42.1_d) << "epsilon=0.1";
	};

	"strings"_test = [] {
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

	"types"_test = [] {
		expect(type<int> != type<long>);
		expect(type<float> != type<double>);

		[[maybe_unused]] const auto i = 0;
		expect(type<const int> == type<decltype(i)>);
		expect(type<decltype(i)> != type<int>);
	};

	"containers"_test = [] {
		std::vector v1 {1, 2, 3};
		std::vector v2 {1, 2, 3};
		expect(v1 == v2);
		expect(std::vector {"a", "b"} != std::vector {"c"});
		expect(std::array {true, false} != std::array {false, true});
	};

	"constant"_test = [] {
		constexpr auto compile_time_v = 42;
		auto run_time_v				  = 99;
		expect(constant<42_i == compile_time_v> and run_time_v == 99_i);
	};

	"convertible"_test = [] {
		expect(bool(std::make_unique<int>()));
		expect(not bool(std::unique_ptr<int> {}));
	};

	"boolean"_test = [] {
		expect("true"_b);
		expect("true"_b or not "true"_b);
		expect((not "true"_b) != "true"_b);
		expect("has value"_b == "value is set"_b);
	};
};

suite suite_array = [] {
	"size"_test = [] {
		const auto size = size_t {5};
		auto tab		= std::array<uint8_t, size> {};
		expect((size == std::size(tab)) >> fatal) << "fatal";
	};
};

suite suite_attr = [] {
	using boost::ut::operator""_test;
	using namespace boost::ut::literals;
	using namespace boost::ut::operators::terse;

	"macro"_test = [] {
#define expect void(),
		expect sum(1, 1) == 2_i;
		expect(6_i == sum(1, 2, 3));
#undef expect
	};

#if __has_cpp_attribute(expect)
	"attribute"_test = [] {
		[[expect]] 3_i == sum(1, 2);
		[[expect]] (sum(1, 2) == 3_i);
	};
#endif
};

suite suite_mut = [] {
	auto i = 0;

	"mut"_test = [i] {
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

suite suite_parameterized = [] {
	for (auto i: std::vector {1, 2, 3}) {
		test("args / " + std::to_string(i)) = [i] { expect(that % i > 0); };
	}

	"args"_test = [](auto arg) { expect(arg > 0_i) << "all values greater than 0"; } | std::vector {1, 2, 3};

	"types"_test = []<class T>() { expect(std::is_integral_v<T>) << "all types are integrals"; } |
				   std::tuple<bool, int> {};

	std::apply(
		[]([[maybe_unused]] auto... args) {
			((test("args and types / " + std::to_string(args)) =
				  [&args] {
					  using TArgs = decltype(args);
					  expect(std::is_integral_v<TArgs> >> fatal);
					  expect(42_i == static_cast<int>(args) or args);
					  expect(type<TArgs> == type<int> or type<TArgs> == type<bool>);
				  }),
			 ...);
		},
		std::tuple {42, true});

	"args and types"_test = []<class TArg>(TArg arg) {
		expect(std::is_integral_v<TArg> >> fatal);
		expect(42_i == static_cast<int>(arg) or arg);
		expect(type<TArg> == type<int> or type<TArg> == type<bool>);
	} | std::tuple {42, true};
};

suite suite_should = [] {
	auto i = 0;
	expect((i == 0_i) >> fatal);

	should("return increased number for ++") = [i] { expect(++mut(i) == 1_i); };
	should("return decreased number for --") = [i]() mutable { expect(--i == -1_i); };
};

suite suite_skip = [] {
	skip / "don't run"_test = [] {
		expect(42_i == 43) << "should not fire!";
		expect(false) << "should fail!";
	};

	skip / test("don't run") = [] {
		expect(42_i == 43) << "should not fire!";
		expect(false) << "should fail!";
	};
};

suite suite_source_location = [] {
	"sl"_test = [] {
		constexpr auto verify = [](auto sl, auto i) { expect(i == 42_i, sl) << "error with given source location"; };
		verify(boost::ut::reflection::source_location::current(), 42_i);
	};
};

suite suite_spec = [] {
	using namespace boost::ut::operators::terse;
	using namespace boost::ut::literals;
	using namespace boost::ut::spec;

	describe("sum") = [] {
		it("should be 0")		  = [] { expect(sum() == 0_i); };
		it("should add all args") = [] { expect(sum(1, 2, 3) == 6_i); };
	};
};

suite suite_tag = [] {
	ut::cfg<override> = {
		.filter = "tag", .tag = {"execute"}, .colors = {.none = "", .pass = "", .fail = ""}, .dry_run = false};

	tag("execute") / skip / "tag"_test = [] {
		expect(42_i == 43) << "should not fire!";
		expect(false) << "should fail!";
	};

	tag("execute") / "tag"_test = [] { expect(42_i == 42); };

	tag("not executed") / "tag"_test = [] { expect(43_i == 42); };

	tag("not executed") / tag("execute") / "tag"_test = [] { expect(42_i == 42); };
};

auto main() -> int
{
	logger::init();

	log_info("\n\nHello, World!\n\n");

	rtos::ThisThread::sleep_for(1s);

	ut::cfg<override> = {.filter = "*", .colors = {.none = "", .pass = "", .fail = ""}, .dry_run = false};

	[[maybe_unused]] const auto result = ut::cfg<>.run({.report_errors = true});

	return static_cast<int>(result);
}
