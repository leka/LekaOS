// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "../lk_imu_kit_fusion/fusion/Fusion.h"
#include "CoreI2C.h"
#include "CoreLSM6DSOX.hpp"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

// ? Example & documentation
// ? https://github.com/xioTechnologies/Fusion/blob/main/Examples/Advanced/main.c

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

auto gyroscope	   = FusionVector {};
auto accelerometer = FusionVector {};

auto pitch = float {};
auto roll  = float {};
auto yaw   = float {};

auto deltaTime = float {};

const auto odr_hz = int {52};

}	// namespace

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	imu::lsm6dsox.init();

	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

	// ? Data: https://www.dropbox.com/scl/fi/cue7qpb77892rozyjbmcq/2022_01_16-IMU-Calibration_Data.xlsx
	// ? gx    0.3166030399
	// ? gy    -0.3415906681
	// ? gz    0.4070272181

	// ? Define calibration
	// const auto gyroscopeOffset = FusionVector {{0.3044174611F, -0.3491523666F, 0.3897702793F}};
	const auto gyroscopeOffset = FusionVector {{}};

	// ? Initialise algorithms
	auto offset = FusionOffset {};
	auto ahrs	= FusionAhrs {};

	FusionOffsetInitialise(&offset, odr_hz);
	FusionAhrsInitialise(&ahrs);

	// ? Set AHRS algorithm settings
	const FusionAhrsSettings settings = {
		.gain				   = 0.5F,
		.accelerationRejection = 10.0F,
		.magneticRejection	   = 0.0F,
		.rejectionTimeout	   = static_cast<unsigned int>(5 * odr_hz),	  // ? 260 samples in 5 seconds
	};

	FusionAhrsSetSettings(&ahrs, &settings);

	auto callback = [&](const interface::LSM6DSOX::SensorData &data) {
		timestamp = rtos::Kernel::Clock::now();

		raw_data = data;

		// ? Acquire latest sensor data
		gyroscope	  = FusionVector {{data.gy.x, data.gy.y, data.gy.z}};
		accelerometer = FusionVector {{data.xl.x, data.xl.y, data.xl.z}};

		// ? Apply calibration
		gyroscope = FusionCalibrationInertial(gyroscope, FUSION_IDENTITY_MATRIX, FUSION_VECTOR_ONES, gyroscopeOffset);

		// ? Update gyroscope offset correction algorithm
		gyroscope = FusionOffsetUpdate(&offset, gyroscope);

		// ? Calculate delta time (in seconds) to account for gyroscope sample clock error
		deltaTime		  = static_cast<float>((timestamp - previousTimestamp).count()) / 1000.F;
		previousTimestamp = timestamp;

		// ? Update gyroscope AHRS algorithm
		FusionAhrsUpdateNoMagnetometer(&ahrs, gyroscope, accelerometer, deltaTime);

		auto euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&ahrs));

		pitch = euler.angle.pitch;
		roll  = euler.angle.roll;
		yaw	  = euler.angle.yaw;
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
