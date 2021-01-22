// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreMotor.h"

namespace leka {

using namespace mbed;

LKCoreMotor::LKCoreMotor(PinName direction_1, PinName direction_2, PinName speed)
{
	// nothing to do
}

void LKCoreMotor::spin(rotation_t rotation, float speed)
{
	return;
}

void LKCoreMotor::stop(void)
{
	return;
}

}	// namespace leka
