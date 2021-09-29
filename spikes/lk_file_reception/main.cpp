// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "FATFileSystem.h"
#include "FileReception.h"
#include "FileSystemKit.h"
#include "HelloWorld.h"
#include "LKCoreFatFs.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "SerialKit.h"

using namespace leka;
using namespace std::chrono;

// Configure CoolTerm in Transmit Options: Use packet delay (40ms) and Transmit Packet Size at 250

rtos::Thread thread_receive_file;

constexpr uint8_t transmit_packet_size = 250;
std::array<uint8_t, transmit_packet_size> buf;

rtos::EventFlags flags_file_reception;

static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
auto serial_kit	   = SerialKit(serial, flags_file_reception);

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

FileSystemKit::File file;
std::string path = "/fs/Three_laws.txt";
size_t file_size = 2'212;

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

	serial_kit.setBufferSize(transmit_packet_size);

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	initializeSD();

	serial_kit.setFilePath(path);
	serial_kit.setExpectedFileSize(file_size);

	rtos::ThisThread::sleep_for(1s);

	thread_receive_file.start({&serial_kit, &SerialKit::receiveFile});

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);
	}
}
