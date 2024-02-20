// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cinttypes>

#include "rtos/ThisThread.h"

#include "CoreI2C.h"
#include "CoreIMU.hpp"
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

}	// namespace

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	HelloWorld hello;
	hello.start();

	imu::coreimu.init();
	imu::coreimu.setPowerMode(CoreIMU::PowerMode::Normal);

	auto callback = [] { log_info("waking up!"); };

	imu::coreimu.enableOnWakeUpInterrupt(callback);

	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
