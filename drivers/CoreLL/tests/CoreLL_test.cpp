// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreLL.h"

#include "gtest/gtest.h"

using namespace leka;

TEST(CoreLLTest, rawMemoryWriteToBuffer)
{
	CoreLL ll;

	uint32_t buffer[5] {0};
	uintptr_t buffer_address = reinterpret_cast<uintptr_t>(&buffer);

	ll.rawMemoryWrite(buffer_address + 0 * sizeof(buffer[0]), 42);
	ll.rawMemoryWrite(buffer_address + 1 * sizeof(buffer[0]), 43);
	ll.rawMemoryWrite(buffer_address + 2 * sizeof(buffer[0]), 44);
	ll.rawMemoryWrite(buffer_address + 3 * sizeof(buffer[0]), 45);
	ll.rawMemoryWrite(buffer_address + 4 * sizeof(buffer[0]), 46);

	ASSERT_EQ(buffer[0], 42);
	ASSERT_EQ(buffer[1], 43);
	ASSERT_EQ(buffer[2], 44);
	ASSERT_EQ(buffer[3], 45);
	ASSERT_EQ(buffer[4], 46);
}
