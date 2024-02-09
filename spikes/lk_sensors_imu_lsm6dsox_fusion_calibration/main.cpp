// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cinttypes>

#include "drivers/HighResClock.h"
#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreLSM6DSOX.hpp"
#include "LogKit.h"

// ? Note the following code has been heavily inspired from:
// ? https://github.com/xioTechnologies/Fusion/blob/main/Examples/Advanced/main.c
// ? For more information, see https://github.com/xioTechnologies/Fusion

#include "fusion/Fusion.h"

using namespace std::chrono;
using namespace leka;

#include "mbed_stats.h"

#include "drivers/Watchdog.h"

namespace {

namespace watchdog {

	namespace internal {

		auto &instance		   = mbed::Watchdog::get_instance();
		constexpr auto timeout = 30000ms;
		auto thread			   = rtos::Thread {osPriorityLow};

		namespace stats {

			auto cpu   = mbed_stats_cpu_t {};
			auto stack = mbed_stats_stack_t {};
			auto heap  = mbed_stats_heap_t {};

		}	// namespace stats

		__attribute__((noreturn)) void watchdog_kick()
		{
			static auto kick_count = uint32_t {0};

			static auto start = rtos::Kernel::Clock::now();
			static auto stop  = rtos::Kernel::Clock::now();
			static auto delta = static_cast<int>((stop - start).count());

			static auto idle_ratio		 = uint8_t {};
			static auto sleep_ratio		 = uint8_t {};
			static auto deep_sleep_ratio = uint8_t {};

			static auto stack_used_delta	= int32_t {};
			static auto stack_used_size		= uint32_t {};
			static auto stack_reserved_size = uint32_t {};
			static auto stack_used_ratio	= uint8_t {};

			static auto heap_used_delta	   = int32_t {};
			static auto heap_used_size	   = uint32_t {};
			static auto heap_reserved_size = uint32_t {};
			static auto heap_used_ratio	   = uint8_t {};

			while (true) {
				internal::instance.kick();
				++kick_count;

				stop  = rtos::Kernel::Clock::now();
				delta = static_cast<int>((stop - start).count());

				mbed_stats_cpu_get(&stats::cpu);

				idle_ratio	= static_cast<uint8_t>(((stats::cpu.idle_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				sleep_ratio = static_cast<uint8_t>(((stats::cpu.sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				deep_sleep_ratio =
					static_cast<uint8_t>(((stats::cpu.deep_sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));

				mbed_stats_stack_get(&stats::stack);

				stack_used_delta	= static_cast<int32_t>(stats::stack.max_size - stack_used_size);
				stack_used_size		= stats::stack.max_size;
				stack_reserved_size = stats::stack.reserved_size;
				stack_used_ratio	= static_cast<uint8_t>((stack_used_size * 100) / stack_reserved_size);

				mbed_stats_heap_get(&stats::heap);

				heap_used_delta	   = static_cast<int32_t>(stats::heap.current_size - heap_used_size);
				heap_used_size	   = stats::heap.current_size;
				heap_reserved_size = stats::heap.reserved_size;
				heap_used_ratio	   = static_cast<uint8_t>((heap_used_size * 100) / heap_reserved_size);

				log_info(
					"dt: %i, kck: %u, idl: %u%%, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
					"%u], hur: %u%% (%+i)[%u/%u]",
					delta, kick_count, idle_ratio, sleep_ratio, deep_sleep_ratio, stack_used_ratio, stack_used_delta,
					stack_used_size, stack_reserved_size, heap_used_ratio, heap_used_delta, heap_used_size,
					heap_reserved_size);

				start = rtos::Kernel::Clock::now();
				rtos::ThisThread::sleep_for(5s);
			}
		}

	}	// namespace internal

	void start()
	{
		internal::instance.start(internal::timeout.count());
		internal::thread.start(watchdog::internal::watchdog_kick);
	}

}	// namespace watchdog

namespace imu {

	namespace internal {

		auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

	}	// namespace internal

	CoreLSM6DSOX lsm6dsox(internal::i2c, internal::drdy_irq);

}	// namespace imu

namespace fusion {

	constexpr auto kODR_HZ = int {52};

	auto ahrs = FusionAhrs {};

	const FusionAhrsSettings settings = {
		.gain				   = 0.5F,
		.accelerationRejection = 10.0F,
		.magneticRejection	   = 0.0F,
		.rejectionTimeout	   = static_cast<unsigned int>(5 * kODR_HZ),   // ? # of samples in 5 seconds
	};

	interface::LSM6DSOX::SensorData::time_point_t timestamp_previous = {};

	auto global_offset = FusionOffset {};

	constexpr auto CALIBRATION = bool {true};
	// constexpr auto CALIBRATION = bool {false};

	void callback(const interface::LSM6DSOX::SensorData data)
	{
		auto timestamp_now	  = data.timestamp;
		auto timestamp_now_us = std::chrono::microseconds {timestamp_now.time_since_epoch()}.count();

		// ? Acquire latest sensor data
		auto gyroscope	   = FusionVector {{data.gy.x, data.gy.y, data.gy.z}};
		auto accelerometer = FusionVector {{data.xl.x, data.xl.y, data.xl.z}};

		if constexpr (CALIBRATION) {
			// ? Define calibration offsets
			// ? Data: https://www.dropbox.com/scl/fi/cue7qpb77892rozyjbmcq/2022_01_16-IMU-Calibration_Data.xlsx
			constexpr auto gyroscope_offset		= FusionVector {{0.02544522554F, -0.3286247803F, 0.3205770357F}};
			constexpr auto accelerometer_offset = FusionVector {{0.006480437024F, -0.01962820621F, 0.003259031049F}};

			// ? Apply calibration offsets
			gyroscope =
				FusionCalibrationInertial(gyroscope, FUSION_IDENTITY_MATRIX, FUSION_VECTOR_ONES, gyroscope_offset);
			accelerometer = FusionCalibrationInertial(accelerometer, FUSION_IDENTITY_MATRIX, FUSION_VECTOR_ONES,
													  accelerometer_offset);

			// ? Update gyroscope offset correction algorithm
			gyroscope = FusionOffsetUpdate(&global_offset, gyroscope);
		}

		// ? Calculate delta time (in seconds) to account for gyroscope sample clock error
		auto delta_time	   = std::chrono::duration<float>(timestamp_now - timestamp_previous).count();
		timestamp_previous = timestamp_now;

		// ? Update gyroscope AHRS algorithm
		FusionAhrsUpdateNoMagnetometer(&ahrs, gyroscope, accelerometer, delta_time);

		// ? Get quaternion
		const auto quaternion = FusionAhrsGetQuaternion(&ahrs);

		const auto q_w = quaternion.element.w;
		const auto q_x = quaternion.element.x;
		const auto q_y = quaternion.element.y;
		const auto q_z = quaternion.element.z;

		// ? Get Euler angles
		const auto euler = FusionQuaternionToEuler(quaternion);

		const auto pitch = euler.angle.pitch;
		const auto roll	 = euler.angle.roll;
		const auto yaw	 = euler.angle.yaw;

		// ? Log values
		// ? See https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.0.pdf#page=24
		log_free("I,%" PRId64 ",%f,%f,%f,%f,%f,%f\r\nQ,%" PRId64 ",%f,%f,%f,%f\r\n", timestamp_now_us, gyroscope.axis.x,
				 gyroscope.axis.y, gyroscope.axis.z, accelerometer.axis.x, accelerometer.axis.y, accelerometer.axis.z,
				 timestamp_now_us, q_w, q_x, q_y, q_z);
	};

}	// namespace fusion

}	// namespace

auto main() -> int
{
	logger::init();
	watchdog::start();

	rtos::ThisThread::sleep_for(1s);

	imu::lsm6dsox.init();

	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

	// ? Initialise algorithms
	FusionAhrsInitialise(&fusion::ahrs);

	if constexpr (fusion::CALIBRATION) {
		// ? Set AHRS algorithm settings
		FusionAhrsSetSettings(&fusion::ahrs, &fusion::settings);

		// ? Set global fusion offset
		FusionOffsetInitialise(&fusion::global_offset, fusion::kODR_HZ);
	}

	rtos::ThisThread::sleep_for(1s);

	imu::lsm6dsox.registerOnGyDataReadyCallback(fusion::callback);
	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

	{
		rtos::ThisThread::sleep_for(5s);
	}

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
