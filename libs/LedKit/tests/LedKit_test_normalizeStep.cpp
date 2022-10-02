// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LedKit.h"
#include "gtest/gtest.h"

using namespace leka::led::animation::utils;

static constexpr auto kStepMax = 100;

TEST(LedKitTestNormalizeStep, normalizeZero)
{
	auto expected_float = 0.F;
	ASSERT_TRUE(normalizeStep(0, kStepMax) == expected_float);
}

TEST(LedKitTestNormalizeStep, normalizeStepMax)
{
	auto expected_float = 1.F;
	ASSERT_TRUE(normalizeStep(kStepMax, kStepMax) == expected_float);
}
