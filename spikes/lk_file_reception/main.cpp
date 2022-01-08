// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "WebKit.h"

using namespace leka;
using namespace std::chrono;

static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
auto web_kit	   = WebKit();

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

void initializeSD()
{
	sd_blockdevice.init();
	sd_blockdevice.frequency(25'000'000);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	auto hello = HelloWorld();

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	initializeSD();

	rtos::ThisThread::sleep_for(1s);

	if (auto is_connected = web_kit.connect("RUT950_LEKA", "Leka2021"); is_connected) {
		log_info("Start downloading file via wifi...");
		web_kit.downloadFile("https://github.com/leka/LekaOS/releases/download/1.2.3/LekaOS-1.2.3.bin",
							 "/fs/os/LekaOS-1.2.3.bin");
	}

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(10s);
	}
}
