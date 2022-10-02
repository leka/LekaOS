// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLL.h"
#include <array>

#include "gtest/gtest.h"

using namespace leka;

TEST(CoreLLTest, rawMemoryWriteToBuffer32bits)
{
	CoreLL ll;

	auto buffer			= std::to_array<uint32_t>({0, 0, 0, 0, 0});
	auto buffer_address = reinterpret_cast<uintptr_t>(buffer.data());

	ll.rawMemoryWrite(buffer_address + 0 * sizeof(buffer[0]), uint32_t {1});
	ll.rawMemoryWrite(buffer_address + 1 * sizeof(buffer[0]), uint32_t {2});
	ll.rawMemoryWrite(buffer_address + 2 * sizeof(buffer[0]), uint32_t {3});
	ll.rawMemoryWrite(buffer_address + 3 * sizeof(buffer[0]), uint32_t {4});
	ll.rawMemoryWrite(buffer_address + 4 * sizeof(buffer[0]), uint32_t {5});

	ASSERT_EQ(buffer[0], 1);
	ASSERT_EQ(buffer[1], 2);
	ASSERT_EQ(buffer[2], 3);
	ASSERT_EQ(buffer[3], 4);
	ASSERT_EQ(buffer[4], 5);
}

TEST(CoreLLTest, rawMemoryWriteToBuffer8bits)
{
	CoreLL ll;

	auto buffer			= std::to_array<uint8_t>({0, 0, 0, 0, 0});
	auto buffer_address = reinterpret_cast<uintptr_t>(buffer.data());

	ll.rawMemoryWrite(buffer_address + 0 * sizeof(buffer[0]), uint8_t {1});
	ll.rawMemoryWrite(buffer_address + 1 * sizeof(buffer[0]), uint8_t {2});
	ll.rawMemoryWrite(buffer_address + 2 * sizeof(buffer[0]), uint8_t {3});
	ll.rawMemoryWrite(buffer_address + 3 * sizeof(buffer[0]), uint8_t {4});
	ll.rawMemoryWrite(buffer_address + 4 * sizeof(buffer[0]), uint8_t {5});

	ASSERT_EQ(buffer[0], 1);
	ASSERT_EQ(buffer[1], 2);
	ASSERT_EQ(buffer[2], 3);
	ASSERT_EQ(buffer[3], 4);
	ASSERT_EQ(buffer[4], 5);
}
