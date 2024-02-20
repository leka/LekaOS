// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cinttypes>

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreIMU.hpp"
#include "CoreLED.h"
#include "CoreSPI.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace std::chrono;
using namespace leka;

namespace {

namespace imu {

	namespace internal {

		auto irq = CoreInterruptIn {PinName::SENSOR_IMU_IRQ};
		auto i2c = CoreI2C {PinName::SENSOR_IMU_TH_I2C_SDA, PinName::SENSOR_IMU_TH_I2C_SCL};

	}	// namespace internal

	CoreIMU coreimu(internal::i2c, internal::irq);

}	// namespace imu

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

}	// namespace leds

}	// namespace

void setColor(RGB color)
{
	leds::ears.setColor(color);
	leds::belt.setColor(color);

	leds::ears.show();
	leds::belt.show();
}

void turnOff()
{
	setColor(RGB::black);
}

void wakeUpReaction()
{
	setColor(RGB::pure_blue);
	rtos::ThisThread::sleep_for(2s);
	turnOff();
}

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	HelloWorld hello;
	hello.start();

	imu::coreimu.init();
	imu::coreimu.setPowerMode(CoreIMU::PowerMode::Normal);

	auto callback = [] { wakeUpReaction(); };

	imu::coreimu.enableOnWakeUpInterrupt(callback);

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
