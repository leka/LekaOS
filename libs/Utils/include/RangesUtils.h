// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <ranges>

namespace leka::utils::ranges {

struct SequenceFromTo {
	int start;
	int end;
};

constexpr auto sequence(const SequenceFromTo seq)
{
	auto values = std::ranges::views::iota(seq.start, seq.end + 1);
	return values;
}

struct SequenceFromSize {
	int from;
	int size;
};

constexpr auto sequence(const SequenceFromSize seq)
{
	auto values = std::ranges::views::iota(seq.from, seq.from + seq.size);
	return values;
}

}	// namespace leka::utils::ranges
