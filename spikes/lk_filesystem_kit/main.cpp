// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "FATFileSystem.h"
#include "FileSystemKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono_literals;

auto file = FileSystemKit::File {};
// char filename[L_tmpnam];
constexpr auto kTmpfile = std::array<char, sizeof("/tmp/XXXXXX")> {"/tmp/XXXXXX"};
auto filename			= std::array<char, L_tmpnam> {};

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

	rtos::ThisThread::sleep_for(1s);

	std::copy(std::begin(kTmpfile), std::end(kTmpfile), filename.begin());
	mkstemp(filename.data());

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(10s);

		auto opened = file.open(filename.data(), "w+");

		if (opened) {
			log_info("File opened");
		} else {
			log_error("Fail to open file");
			break;
		}

		auto input_data = std::to_array({'h', 'e', 'l', 'l', 'o', ' ', 'l', 'e', 'k', 'a'});
		char *buffer	= nullptr;

		auto bytes_written = file.write(input_data);
		if (bytes_written != 0U) {
			log_info("File edited");
		} else {
			log_error("Fail to edit file");
			break;
		}

		auto size = file.size();
		buffer	  = static_cast<char *>(malloc(sizeof(char) * size));
		if (buffer == nullptr) {
			log_error("Memory error");
			break;
		}

		file.rewind();
		auto bytes_read = file.read(buffer, size);
		if (bytes_read != 0U) {
			log_info("Reading...");
			log_info("Data : %s", buffer);
		} else {
			log_error("Fail to read file");
			break;
		}

		file.seek(6);
		bytes_read = file.read(buffer, size);
		if (bytes_read != 0U) {
			log_info("Reading...");
			log_info("Data after seeking: %s", buffer);
		} else {
			log_error("Fail to read file");
			break;
		}

		file.close();

		free(buffer);
	}

	return 0;
}
