#include "LKCalculatorKit.h"

using namespace leka;

int LKCalculatorKit::add(int x, int y)
{
	_last_result = x + y;
	return _last_result;
}

int LKCalculatorKit::multiply(int x, int y)
{
	_last_result = x * y;
	return _last_result;
}

int LKCalculatorKit::getLastResult(void)
{
	return _last_result;
}
