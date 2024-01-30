// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "Utils.h"
#include "gtest/gtest.h"

using namespace leka::utils::ranges;

TEST(RangesTests, fromStart0ToEnd5)
{
	auto expected = std::to_array({0, 1, 2, 3, 4, 5});

	auto i = 0;
	for (auto value: sequence({.start = 0, .end = 5})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 5 + 1) << "Loop ran " << i << " times, should run " << 5 + 1 << " times";
}

TEST(RangesTests, fromStart10ToEnd20)
{
	auto expected = std::to_array({10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20});

	auto i = 0;
	for (auto value: sequence({.start = 10, .end = 20})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 10 + 1) << "Loop ran " << i << " times, should run " << 10 + 1 << " times";
}

TEST(RangesTests, fromStart0ToEnd0)
{
	auto expected = std::to_array({0});

	auto i = 0;
	for (auto value: sequence({.start = 0, .end = 0})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 1) << "Loop ran " << i << " times, should run " << 0 + 1 << " times";
}

TEST(RangesTests, fromStart1ToEnd1)
{
	auto expected = std::to_array({1});

	auto i = 0;
	for (auto value: sequence({.start = 1, .end = 1})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 1) << "Loop ran " << i << " times, should run " << 0 + 1 << " times";
}

TEST(RangesTests, from0Size5)
{
	auto expected = std::to_array({0, 1, 2, 3, 4});

	auto i = 0;
	for (auto value: sequence({.from = 0, .size = 5})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 5) << "Loop ran " << i << " times, should run " << 5 << " times";
}

TEST(RangesTests, from10Size10)
{
	auto expected = std::to_array({10, 11, 12, 13, 14, 15, 16, 17, 18, 19});

	auto i = 0;
	for (auto value: sequence({.from = 10, .size = 10})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 10) << "Loop ran " << i << " times, should run " << 10 << " times";
}

TEST(RangesTests, from0Size0)
{
	auto expected = std::to_array({0});

	auto i = 0;
	for (auto value: sequence({.from = 0, .size = 0})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 0) << "Loop ran " << i << " times, should run " << 0 << " times";
}

TEST(RangesTests, from0Size1)
{
	auto expected = std::to_array({0});

	auto i = 0;
	for (auto value: sequence({.from = 0, .size = 1})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 1) << "Loop ran " << i << " times, should run " << 1 << " times";
}

TEST(RangesTests, from1Size0)
{
	auto expected = std::to_array({1});

	auto i = 0;
	for (auto value: sequence({.from = 1, .size = 0})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 0) << "Loop ran " << i << " times, should run " << 0 << " times";
}

TEST(RangesTests, from1Size1)
{
	auto expected = std::to_array({1});

	auto i = 0;
	for (auto value: sequence({.from = 1, .size = 1})) {
		EXPECT_EQ(value, expected.at(i));
		++i;
	}

	EXPECT_EQ(i, 1) << "Loop ran " << i << " times, should run " << 1 << " times";
}
