// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreLSM6DSOX.h"
#include "IMUKit.h"
#include "tests/config.h"

using namespace leka;
using namespace std::chrono;
using namespace boost::ut;
using namespace boost::ut::bdd;

suite suite_imu_kit = [] {
	constexpr auto default_min_bound_pitch		 = -5._f;
	constexpr auto default_max_bound_pitch		 = 5._f;
	constexpr auto default_min_bound_roll		 = -5._f;
	constexpr auto default_max_bound_roll		 = 5._f;
	constexpr auto default_min_bound_yaw		 = 170._f;
	constexpr auto default_max_bound_yaw		 = 190._f;
	constexpr auto measurement_iteration		 = 100;
	constexpr auto maximal_pitch_noise_amplitude = 0.5F;
	constexpr auto maximal_roll_noise_amplitude	 = 0.5F;
	constexpr auto maximal_yaw_drift			 = 15.F;

	auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);
	auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
	auto lsm6dsox = CoreLSM6DSOX {i2c, drdy_irq};
	auto imukit	  = IMUKit {lsm6dsox};

	lsm6dsox.init();

	"Initialization"_test = [&] {
		expect(neq(&imukit, nullptr));
		imukit.init();
		imukit.start();
	};

	scenario("imu - get angles") = [&] {
		when("robot on horizontal position") = [&] {
			then("I expect pitch to be close to 0") = [&] {
				auto pitch = imukit.getAngles().at(0);
				expect(ge(pitch, default_min_bound_pitch)) << " (" << pitch << " < " << default_min_bound_pitch << ")";
				expect(le(pitch, default_max_bound_pitch)) << " (" << pitch << " > " << default_max_bound_pitch << ")";
			};

			then("I expect roll to be close to 0") = [&] {
				auto roll = imukit.getAngles().at(1);
				expect(ge(roll, default_min_bound_roll)) << " (" << roll << " < " << default_min_bound_roll << ")";
				expect(le(roll, default_max_bound_roll)) << " (" << roll << " > " << default_max_bound_roll << ")";
			};

			then("I expect yaw to be close to 180") = [&] {
				auto yaw = imukit.getAngles().at(2);
				expect(ge(yaw, default_min_bound_yaw)) << " (" << yaw << " < " << default_min_bound_yaw << ")";
				expect(le(yaw, default_max_bound_yaw)) << " (" << yaw << " > " << default_max_bound_yaw << ")";
			};
		};
	};

	scenario("imu - measurement stability") = [&] {
		given("a new origin is set") = [&] {
			imukit.setOrigin();

			then("I expect yaw to be reset to 180 degrees") = [&] {
				auto [pitch, roll, yaw] = imukit.getAngles();
				expect(yaw > default_min_bound_yaw)
					<< "Yaw (" << yaw << ") lesser than minimal bound (" << default_min_bound_yaw << ")";
				expect(yaw < default_max_bound_yaw)
					<< "Yaw (" << yaw << ") greater than maximal bound (" << default_max_bound_yaw << ")";
			};
			when("I wait for 10 seconds") = [&] {
				auto [first_pitch, first_roll, first_yaw] = imukit.getAngles();

				rtos::ThisThread::sleep_for(10s);

				auto [current_pitch, current_roll, current_yaw] = imukit.getAngles();

				auto pitch_drift = first_pitch - current_pitch;
				auto roll_drift	 = first_roll - current_roll;
				auto yaw_drift	 = first_yaw - current_yaw;

				then("I expect pitch NOT to drift") = [&] {
					expect(le(pitch_drift, maximal_pitch_noise_amplitude))
						<< "(" << pitch_drift << " > " << maximal_pitch_noise_amplitude << ")";
				};
				then("I expect roll NOT to drift") = [&] {
					expect(le(roll_drift, maximal_roll_noise_amplitude))
						<< "(" << roll_drift << " > " << maximal_roll_noise_amplitude << ")";
				};
				then("I expect yaw to drift slightly") = [&] {
					expect(le(yaw_drift, maximal_yaw_drift)) << "(" << yaw_drift << " > " << maximal_yaw_drift << ")";
				};
			};
		};
	};
};
