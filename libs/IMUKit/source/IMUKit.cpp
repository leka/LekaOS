// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "IMUKit.hpp"

#include "rtos/Kernel.h"

#include "fusion/Fusion.h"

// ? Note the following code has been heavily inspired from:
// ? https://github.com/xioTechnologies/Fusion/blob/main/Examples/Advanced/main.c
// ? For more information, see https://github.com/xioTechnologies/Fusion

using namespace std::chrono;
using namespace leka;

namespace fusion {

constexpr auto kODR_HZ = int {52};

static auto ahrs = FusionAhrs {};

const FusionAhrsSettings settings = {
	.gain				   = 0.5F,
	.accelerationRejection = 10.0F,
	.magneticRejection	   = 0.0F,
	.rejectionTimeout	   = static_cast<unsigned int>(5 * kODR_HZ),   // # of samples in 5 seconds
};

// ? Define calibration offsets
// ? Data: https://www.dropbox.com/scl/fi/cue7qpb77892rozyjbmcq/2022_01_16-IMU-Calibration_Data.xlsx
constexpr auto gyroscope_offset		= FusionVector {{0.02544522554F, -0.3286247803F, 0.3205770357F}};
constexpr auto accelerometer_offset = FusionVector {{0.006480437024F, -0.01962820621F, 0.003259031049F}};

auto timestamp_now		= rtos::Kernel::Clock::now();
auto timestamp_previous = rtos::Kernel::Clock::now();

auto gyroscope	   = FusionVector {};
auto accelerometer = FusionVector {};

auto global_offset = FusionOffset {};

constexpr auto CALIBRATION = bool {true};

}	// namespace fusion

void IMUKit::init()
{
	// ? Initialise algorithms
	FusionAhrsInitialise(&fusion::ahrs);

	if constexpr (fusion::CALIBRATION) {
		FusionAhrsSetSettings(&fusion::ahrs, &fusion::settings);
		FusionOffsetInitialise(&fusion::global_offset, fusion::kODR_HZ);
	}

	auto on_drdy_callback = [this](const interface::LSM6DSOX::SensorData &data) { drdy_callback(data); };

	_lsm6dsox.registerOnGyDataReadyCallback(on_drdy_callback);
}

void IMUKit::start()
{
	_lsm6dsox.setPowerMode(interface::LSM6DSOX::PowerMode::Normal);
}

void IMUKit::stop()
{
	_lsm6dsox.setPowerMode(interface::LSM6DSOX::PowerMode::Off);
}

void IMUKit::setOrigin()
{
	// TODO(@ladislas): to implement
}

auto IMUKit::getEulerAngles() -> const EulerAngles &
{
	return _euler_angles;
}

void IMUKit::drdy_callback(const interface::LSM6DSOX::SensorData &data)
{
	// ? Note: For a detailed explanation on the code below, checkout
	// ? https://github.com/leka/LekaOS/tree/develop/spikes/lk_sensors_imu_lsm6dsox_fusion_calibration

	fusion::timestamp_now = rtos::Kernel::Clock::now();

	fusion::gyroscope	  = FusionVector {{data.gy.x, data.gy.y, data.gy.z}};
	fusion::accelerometer = FusionVector {{data.xl.x, data.xl.y, data.xl.z}};

	if constexpr (fusion::CALIBRATION) {
		fusion::gyroscope	  = FusionCalibrationInertial(fusion::gyroscope, FUSION_IDENTITY_MATRIX, FUSION_VECTOR_ONES,
														  fusion::gyroscope_offset);
		fusion::accelerometer = FusionCalibrationInertial(fusion::accelerometer, FUSION_IDENTITY_MATRIX,
														  FUSION_VECTOR_ONES, fusion::accelerometer_offset);

		fusion::gyroscope = FusionOffsetUpdate(&fusion::global_offset, fusion::gyroscope);
	}

	auto delta_time = static_cast<float>((fusion::timestamp_now - fusion::timestamp_previous).count()) / 1000.F;
	fusion::timestamp_previous = fusion::timestamp_now;

	FusionAhrsUpdateNoMagnetometer(&fusion::ahrs, fusion::gyroscope, fusion::accelerometer, delta_time);

	const auto euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&fusion::ahrs));
	_euler_angles	 = {
		   .pitch = euler.angle.pitch,
		   .roll  = euler.angle.roll,
		   .yaw	  = euler.angle.yaw,
	   };
};
