// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreAccelerometer.h"
#include "CoreGyroscope.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

namespace imu {

	namespace internal {

		CoreI2C i2c(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
		CoreLSM6DSOX lsm6dsox(internal::i2c);

	}	// namespace internal

	CoreAccelerometer accel(internal::lsm6dsox);
	CoreGyroscope gyro(internal::lsm6dsox);

}	// namespace imu

}	// namespace

const auto SLEEP_DELAY = 50ms;
auto counter		   = 0;

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	imu::internal::lsm6dsox.init();

	rtos::ThisThread::sleep_for(1s);
	log_free("#, ts, dt, ax, ay, az, gx, gy, gz\n");
	rtos::ThisThread::sleep_for(500ms);

	while (true) {
		const auto start = rtos::Kernel::Clock::now();

		auto [gx, gy, gz] = imu::gyro.getXYZ();
		auto [ax, ay, az] = imu::accel.getXYZ();

		const auto stop = rtos::Kernel::Clock::now();

		const auto delta = static_cast<int>((stop - start).count());
		const auto now	 = static_cast<int>(start.time_since_epoch().count());

		log_free("%i, %i, %i, %f, %f, %f, %f, %f, %f\n", counter, now, delta, ax, ay, az, gx, gy, gz);

		++counter;
		rtos::ThisThread::sleep_for(SLEEP_DELAY);
	}
}
