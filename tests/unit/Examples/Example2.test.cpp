#include "utest.h"

int add(int x, int y)
{
	return x + y;
}

UTEST(Example2, first)
{
	ASSERT_TRUE(1);
}

UTEST(Example2, second)
{
	ASSERT_FALSE(false);
}

UTEST(Example2, addition)
{
	ASSERT_EQ(12, add(4, 8));
	ASSERT_NE(10, add(2, 7));
}
