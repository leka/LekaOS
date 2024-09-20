// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <array>

#include "gtest/gtest.h"
#include "interface/Behavior.h"

using namespace leka;

TEST(BehaviorIDTest, behaviorsHaveUniqueID)
{
	// auto all_behaviors = std::to_array<interface::Behavior *>({});
	std::array<interface::Behavior *, 0> all_behaviors =
		{};	  // TODO: to remove, only for empty list, use above instead

	auto comparator = [](interface::Behavior *a, interface::Behavior *b) { return a->id() < b->id(); };
	std::sort(all_behaviors.begin(), all_behaviors.end(), comparator);

	auto array_size = std::size(all_behaviors);
	if (array_size <= 1) {
		return;
	}

	for (auto i = 1; i < array_size; i++) {
		ASSERT_NE(all_behaviors.at(i - 1)->id(), all_behaviors.at(i)->id());
	}
}
