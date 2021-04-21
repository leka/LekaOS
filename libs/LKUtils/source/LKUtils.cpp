// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKUtils.h"

leka::utils::LinearFunctionValues leka::utils::linearFunction(float x1, float x2, float y1, float y2)
{
	leka::utils::LinearFunctionValues values {0, 0};

	if (x2 - x1 == 0) {
		return values;
	}

	values.slope	   = (y2 - y1) / (x2 - x1);
	values.y_intercept = y1 - values.slope * x1;

	return values;
}
