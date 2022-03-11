// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"

#include "CoreBattery.h"
#include "CoreTimeout.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "RobotController.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

auto sleep_timeout = CoreTimeout {};

auto charge_input = mbed::InterruptIn {PinName::BATTERY_CHARGE_STATUS};
auto battery	  = leka::CoreBattery {PinName::BATTERY_VOLTAGE, charge_input};

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

auto rc = RobotController {sleep_timeout, battery};

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	logger::init();

	rtos::ThisThread::sleep_for(1s);

	log_info("\n\n");
	log_info("Hello, LekaOS!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	initializeSD();

	rc.initializeComponents();
	rc.registerEvents();

	while (true) {
		log_debug("A message from your board %s --> \"%s\" at %ims", MBED_CONF_APP_TARGET_NAME, hello.world,
				  int(rtos::Kernel::Clock::now().time_since_epoch().count()));
		rtos::ThisThread::sleep_for(1s);
	}
}
