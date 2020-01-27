#include "gtest/gtest.h"

TEST(TextExample, firstTest) {
	int val = 4;

	EXPECT_EQ(val, 4);
	EXPECT_NE(val, 3);
}
