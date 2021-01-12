// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCalculatorKit.h"

#include "gtest/gtest.h"

// Test list
// - create object - done
// - add two numbers - done
// - multiply two numbers - done
// - get last result - done

using namespace leka;

class LKCalculatorKitTest : public ::testing::Test
{
  protected:
	// void SetUp() override {}
	// void TearDown() override {}

	LKCalculatorKit calc;
};

LKCalculatorKit calc;

TEST_F(LKCalculatorKitTest, initialization)
{
	ASSERT_NE(&calc, nullptr);
}

TEST_F(LKCalculatorKitTest, addTwoNumbers)
{
	auto result = calc.add(3, 4);
	ASSERT_EQ(result, 3 + 4);
}

TEST_F(LKCalculatorKitTest, multiplyTwoNumbers)
{
	auto result = calc.multiply(2, 5);
	ASSERT_EQ(result, 2 * 5);
}

TEST_F(LKCalculatorKitTest, getLastResultWithNoPriorCalculations)
{
	ASSERT_EQ(calc.getLastResult(), 0);
}

TEST_F(LKCalculatorKitTest, getLastResultWithPriorCalculations)
{
	calc.multiply(2, 2);
	calc.multiply(2, 3);

	auto result = calc.multiply(2, 4);
	ASSERT_EQ(result, 2 * 4);

	auto last_result = calc.getLastResult();
	ASSERT_EQ(last_result, result);

	calc.add(2, 2);
	calc.add(2, 3);

	result = calc.add(2, 4);
	ASSERT_EQ(result, 2 + 4);

	last_result = calc.getLastResult();
	ASSERT_EQ(last_result, result);
}
