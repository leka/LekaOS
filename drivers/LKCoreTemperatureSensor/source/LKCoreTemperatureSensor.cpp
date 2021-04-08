// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreTemperatureSensor.h"

namespace leka {
LKCoreTemperatureSensor::LKCoreTemperatureSensor(mbed::I2C &i2c)
	: LKCoreHTS221Driver(i2c), LKCoreTemperatureSensorDriverBase()
{
	init();
	calibration();
};
}	// namespace leka
