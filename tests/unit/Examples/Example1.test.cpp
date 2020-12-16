#include "utest.h"

UTEST(Example1, first)
{
	ASSERT_TRUE(1);
}

UTEST(Example1, second)
{
	ASSERT_FALSE(false);
}
