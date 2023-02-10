// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreLED.h"
#include "CoreLSM6DSOX.hpp"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

namespace leds {

	namespace internal {

		namespace ears {

			auto spi			= CoreSPI {LED_EARS_SPI_MOSI, NC, LED_EARS_SPI_SCK};
			constexpr auto size = 2;

		}	// namespace ears

		namespace belt {

			auto spi			= CoreSPI {LED_BELT_SPI_MOSI, NC, LED_BELT_SPI_SCK};
			constexpr auto size = 20;

		}	// namespace belt

	}	// namespace internal

	auto ears = CoreLED<internal::ears::size> {internal::ears::spi};
	auto belt = CoreLED<internal::belt::size> {internal::belt::spi};

	void turnOff()
	{
		ears.setColor(RGB::black);
		belt.setColor(RGB::black);
		ears.show();
		belt.show();
	}

}	// namespace leds

namespace imu {

	namespace internal {

		auto drdy_irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		auto i2c	  = CoreI2C(PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL);

	}	// namespace internal

	CoreLSM6DSOX lsm6dsox(internal::i2c, internal::drdy_irq);

}	// namespace imu

}	// namespace

auto main() -> int
{
	logger::init();

	HelloWorld hello;
	hello.start();

	imu::lsm6dsox.init();

	imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Off);

	auto callback = [](const interface::LSM6DSOX::SensorData &imu_data) {
		const auto &[xlx, xly, xlz] = imu_data.xl;
		const auto &[gx, gy, gz]	= imu_data.gy;

		log_debug("xl.x: %7.2f, xl.y: %7.2f, xl.z: %7.2f, gy.x: %7.2f, gy.y: %7.2f, gy.z: %7.2f", xlx, xly, xlz, gx, gy,
				  gz);
	};

	imu::lsm6dsox.registerOnGyDataReadyCallback(callback);

	auto double_tap_callback = [] {
		leds::ears.setColor(RGB::cyan);
		leds::belt.setColor(RGB::cyan);

		leds::ears.show();
		leds::belt.show();
		rtos::ThisThread::sleep_for(100ms);

		imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

		leds::turnOff();
	};

	leds::ears.setColor(RGB::cyan);
	leds::belt.setColor(RGB::cyan);

	imu::lsm6dsox.registerOnDoubleTapCallback(double_tap_callback);

	while (true) {
		log_info("Setting normal power mode for 5s");
		rtos::ThisThread::sleep_for(1s);
		imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::Normal);

		rtos::ThisThread::sleep_for(5s);

		imu::lsm6dsox.setPowerMode(CoreLSM6DSOX::PowerMode::UltraLow);
		rtos::ThisThread::sleep_for(500ms);
		log_info("Turning UltraLow for 5s");

		rtos::ThisThread::sleep_for(5s);
	}
}
