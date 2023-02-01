// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreLSM6DSOX.hpp"
#include "LogKit.h"
#include "internal/Mahony.hpp"

using namespace std::chrono;
using namespace leka;

namespace {

namespace imu {

	namespace internal {

		auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

	}	// namespace internal

	CoreLSM6DSOX lsm6dsox(internal::i2c, internal::drdy_irq);

}	// namespace imu

auto timestamp		   = rtos::Kernel::Clock::now();
auto previousTimestamp = rtos::Kernel::Clock::now();

auto raw_data = interface::LSM6DSOX::SensorData {
};

auto pitch = float {};
auto roll  = float {};
auto yaw   = float {};

auto deltaTime = float {};

}	// namespace

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	imu::lsm6dsox.init();

	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

	ahrs::Mahony _mahony {};

	auto callback = [&](const interface::LSM6DSOX::SensorData &data) {
		timestamp = rtos::Kernel::Clock::now();

		raw_data = data;

		// ? Acquire latest sensor data

		auto xl_data			= std::make_tuple(data.xl.x, data.xl.y, data.xl.z);
		auto gy_data			= std::make_tuple(data.gy.x, data.gy.y, data.gy.z);
		constexpr auto mag_data = std::make_tuple(0.0F, 0.0F, 0.0F);

		// ? Calculate delta time (in seconds) to account for gyroscope sample clock error
		deltaTime		  = static_cast<float>((timestamp - previousTimestamp).count()) / 1000.F;
		previousTimestamp = timestamp;

		// ? Update gyroscope AHRS algorithm
		_mahony.update(xl_data, gy_data, mag_data, deltaTime);

		pitch = _mahony.getPitch();
		roll  = _mahony.getRoll();
		yaw	  = _mahony.getYaw();
	};

	rtos::ThisThread::sleep_for(1s);

	imu::lsm6dsox.registerOnGyDataReadyCallback(callback);

	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

	while (true) {
		// ? Log values
		rtos::ThisThread::sleep_for(100ms);

		const auto &[gy_x, gy_y, gy_z] = raw_data.gy;
		const auto &[xl_x, xl_y, xl_z] = raw_data.xl;

		log_free(
			"dt: %f, ax: %7.2f, ay: %7.2f, az: %7.2f, gx: %7.2f, gy: %7.2f, gz: %7.2f, p: %7.2f, r: %7.2f, y: "
			"%7.2f\r",
			deltaTime, xl_x, xl_y, xl_z, gy_x, gy_y, gy_z, pitch, roll, yaw);
	}
}
