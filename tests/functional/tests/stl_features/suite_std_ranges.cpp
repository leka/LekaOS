// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <ranges>

#include "RangesUtils.h"
#include "tests/config.h"

using namespace boost::ut;
using namespace leka;

constexpr auto std_ranges_iota_generator = [](auto a, auto b) {
	auto x = std::ranges::views::iota(a) | std::ranges::views::take(b - a + 1);
	return std::ranges::views::all(x);
};

suite suite_std_ranges = [] {
	"generate range of 5 values from 1 to 5"_test = [] {
		auto data = std::to_array({1, 2, 3, 4, 5});
		auto i	  = 0;
		for (auto v: std_ranges_iota_generator(1, 5)) {
			expect(data[i] == v);
			++i;
		}
	};

	"store range in variable and compare"_test = [&] {
		auto data = std_ranges_iota_generator(10, 20);
		auto i	  = 0;

		for (auto v: std_ranges_iota_generator(10, 20)) {
			expect(data[i] == v);
			++i;
		}
	};
};

suite suite_utils_ranges = [] {
	"generate range of 6 values from 0 to 5 included"_test = [] {
		auto expected = std::to_array({0, 1, 2, 3, 4, 5});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.start = 0, .end = 5})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 5 + 1) << "Loop ran " << i << " times, should run " << 5 + 1 << " times";
	};

	"generate range of 11 values from 10 to 20 included"_test = [] {
		auto expected = std::to_array({0, 1, 2, 3, 4, 5});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.start = 0, .end = 5})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 5 + 1) << "Loop ran " << i << " times, should run " << 5 + 1 << " times";
	};

	"generate range of 1 value from 0 to 0 included"_test = [] {
		auto expected = std::to_array({0});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.start = 0, .end = 0})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 1) << "Loop ran " << i << " times, should run " << 0 << " times";
	};

	"generate range of 1 value from 1 to 1 included"_test = [] {
		auto expected = std::to_array({1});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.start = 1, .end = 1})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 1) << "Loop ran " << i << " times, should run " << 0 << " times";
	};

	"generate range of 5 values starting from 0"_test = [] {
		auto expected = std::to_array({0, 1, 2, 3, 4, 5});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.from = 0, .size = 5})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 5) << "Loop ran " << i << " times, should run " << 5 << " times";
	};

	"generate range of 10 values starting from 10"_test = [] {
		auto expected = std::to_array({10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.from = 10, .size = 10})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 10) << "Loop ran " << i << " times, should run " << 10 << " times";
	};

	"generate range of 0 values starting from 0"_test = [] {
		auto expected = std::to_array({0});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.from = 0, .size = 0})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 0) << "Loop ran " << i << " times, should run " << 0 << " times";
	};

	"generate range of 0 values starting from 1"_test = [] {
		auto expected = std::to_array({1});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.from = 1, .size = 0})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 0) << "Loop ran " << i << " times, should run " << 0 << " times";
	};

	"generate range of 1 values starting from 0"_test = [] {
		auto expected = std::to_array({0});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.from = 0, .size = 1})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 1) << "Loop ran " << i << " times, should run " << 1 << " times";
	};

	"generate range of 1 values starting from 1"_test = [] {
		auto expected = std::to_array({1});

		auto i = 0;
		for (auto value: utils::ranges::sequence({.from = 1, .size = 1})) {
			expect(value == expected.at(i));
			++i;
		}

		expect(i == 1) << "Loop ran " << i << " times, should run " << 1 << " times";
	};
};
