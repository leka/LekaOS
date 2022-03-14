// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "ConfigKit.h"
#include "ConfigList.h"
#include "FATFileSystem.h"
#include "FileSystemKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono_literals;

auto file				= FileSystemKit::File {};
auto filename			= std::to_array("/fs/tmp/test_spike_lk_filesystem_kit");
const uint8_t seek_temp = 7;
const uint8_t h_offset	= 5;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

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

	FileSystemKit::config::set("/fs/conf/bootloader_launch.conf", 0);

	rtos::ThisThread::sleep_for(1s);

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		FileSystemKit::config::set(FileSystemKit::config::hysteresis, h_offset);
		auto battery_level_hysteresis_offset = FileSystemKit::config::get(FileSystemKit::config::hysteresis);
		log_info("Battery Level Hysteresis : %d", battery_level_hysteresis_offset);

		auto launch_count = FileSystemKit::config::get("/fs/conf/bootloader_launch.conf");
		log_info("Launch Count : %d", launch_count);
		FileSystemKit::config::set("/fs/conf/bootloader_launch.conf", launch_count + 1);

		if (auto open = file.open(filename.data(), "w"); !open) {
			log_error("Fail to open file");
			return EXIT_FAILURE;
		}

		log_info("File opened in mode 'w'");

		auto buffer = std::array<char, BUFSIZ> {};

		if (auto buffering = file.setBuffer(buffer); !buffering) {
			log_error("Fail to set buffer");
			return EXIT_FAILURE;
		}

		log_info("Setting buffer");

		auto input_data = std::to_array("Hello, Leka!");

		if (auto bytes = file.write(input_data); bytes != std::size(input_data)) {
			log_error("Fail to edit file");
			return EXIT_FAILURE;
		}
		log_info("File edited");

		auto size = file.size();
		if (size != std::size(input_data)) {
			log_error("Size error");
			return EXIT_FAILURE;
		}
		log_info("Size : %d", size);

		auto output_data = std::array<char, 1024> {};

		if (auto open = file.reopen(filename.data(), "r"); !open) {
			log_error("Fail to re-open file");
			return EXIT_FAILURE;
		}
		log_info("File re-opened in mode 'r'");

		file.rewind();
		log_info("Reading...");
		if (auto bytes = file.read(output_data); bytes != size) {
			log_error("Fail to read file");
			return EXIT_FAILURE;
		}
		log_info("File read");
		log_info("Data : %s", output_data.data());

		file.seek(seek_temp);
		log_info("Position indicator set to %d", file.tell());
		log_info("Reading...");
		if (auto bytes = file.read(output_data); bytes != (size - seek_temp)) {
			log_error("Fail to read file");
			return EXIT_FAILURE;
		}
		log_info("File read");
		log_info("Data : %s", output_data.data());

		file.close();
		log_info("File closed");
		rtos::ThisThread::sleep_for(10s);
	}
}
