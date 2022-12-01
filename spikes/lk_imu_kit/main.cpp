// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreAccelerometer.h"
#include "CoreGyroscope.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "EventLoopKit.h"
#include "HelloWorld.h"
#include "IMUKit.h"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

namespace imu {

	namespace internal {

		CoreI2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
		CoreLSM6DSOX lsm6dsox(internal::i2c);
		EventLoopKit event_loop {};

	}	// namespace internal

	CoreAccelerometer accel(internal::lsm6dsox);
	CoreGyroscope gyro(internal::lsm6dsox);

	IMUKit kit(internal::event_loop, accel, gyro);

}	// namespace imu

}	// namespace

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::internal::lsm6dsox.init();

	imu::kit.init();
	imu::kit.start();

	while (true) {
		auto [pitch, roll, yaw] = imu::kit.getAngles();
		log_info("Pitch : %7.2f, Roll : %7.2f Yaw : %7.2f", pitch, roll, yaw);

		rtos::ThisThread::sleep_for(140ms);
	}
}
