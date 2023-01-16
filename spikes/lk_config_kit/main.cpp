// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>
#include <filesystem>

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "ConfigKit.h"
#include "ConfigList.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono_literals;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

auto configkit		= ConfigKit();
auto config_to_edit = Config {"/fs/var/tmp/config"};

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

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	auto hello = HelloWorld();

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	initializeSD();

	auto custom_data = uint8_t {0};

	rtos::ThisThread::sleep_for(1s);

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		++custom_data;
		if (auto write = configkit.write(config_to_edit, {custom_data}); !write) {
			log_error("Fail to write config file");
			return EXIT_FAILURE;
		}
		auto config_value = configkit.read(config_to_edit);
		log_info("Config value : %d", config_value);

		rtos::ThisThread::sleep_for(10s);
	}
}
