#include "rtos/ThisThread.h"

#include "utest.h"

UTEST(Example3, first)
{
	rtos::ThisThread::sleep_for(500);
	ASSERT_TRUE(1);
}

UTEST(Example3, second)
{
	ASSERT_EQ(true, true);
}
