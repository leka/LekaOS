// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "FileManager.h"
#include "FileSystemKit.h"
#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto main() -> int
{
	logger::init();

	auto start = rtos::Kernel::Clock::now();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	HelloWorld hello;
	hello.start();

	FileManager sd_card;
	sd_card.testWorkingToRemove();

	const auto filename = std::array<char, 32> {"assets/images/emotion-happy.jpg"};
	sd_card.open(filename.data());

	uint32_t file_size = sd_card.getFileSize();
	log_info("File %s opened. File size : %lu bytes\n", filename.data(), file_size);

	sd_card.close();

	const auto *filename2 = "/fs/assets/images/emotion-happy.jpg";

	if (FileSystemKit::File file; file.open(filename2)) {
		log_info("FileSystemKit::File opened %s. Size : %lu bytes\n", filename2, file.size());
	} else {
		log_info("FileSystemKit::File failed to open %s\n", filename2);
	}

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(10s);
	}
}
