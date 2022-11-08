// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreAccelerometer.h"
#include "CoreGyroscope.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

namespace imu {

	CoreI2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	CoreLSM6DSOX lsm6dsox(i2c);
	CoreAccelerometer accel(lsm6dsox);
	CoreGyroscope gyro(lsm6dsox);

}	// namespace imu

}	// namespace

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::lsm6dsox.init();

	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

	while (true) {
		auto [xlx, xly, xlz] = imu::accel.getXYZ();
		auto [gyx, gyy, gyz] = imu::gyro.getXYZ();

		log_info("Ax: %f, Ay: %f, Az: %f, Gx: %f, Gy: %f, Gz: %f)", xlx, xly, xlz, gyx, gyy, gyz);

		rtos::ThisThread::sleep_for(250ms);
	}
}
