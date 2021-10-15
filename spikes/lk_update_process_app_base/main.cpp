// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreQSPI.h"
#include "FATFileSystem.h"
#include "FileSystemKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "QSPIFBlockDevice.h"
#include "SDBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "bootutil/bootutil.h"

using namespace leka;
using namespace std::chrono;

HelloWorld hello;

static mbed::BufferedSerial serial(USBTX, USBRX, 115200);

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

auto coreqspi		   = CoreQSPI();
auto coremanageris25lp = CoreFlashManagerIS25LP016D(coreqspi);
auto coreis25lp		   = CoreFlashIS25LP016D(coreqspi, coremanageris25lp);

auto get_secondary_bd() -> mbed::BlockDevice *
{
	static auto _bd = QSPIFBlockDevice {};

	static auto sliced_bd = mbed::SlicingBlockDevice {&_bd, 0x0, MCUBOOT_SLOT_SIZE};

	return &sliced_bd;
}

void initializeSD()
{
	sd_blockdevice.init();
	sd_blockdevice.frequency(25'000'000);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });
	log_info("Hello, Application!\n");

	// Initialization
	hello.start();

	initializeSD();

	coreis25lp.reset();
	coreqspi.setDataTransmissionFormat();
	coreqspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);
	coreis25lp.erase();

	// Open file and initialize tools
	uint32_t address		 = 0x0;
	const size_t packet_size = 0x100;
	auto packet_read		 = packet_size;
	std::array<uint8_t, packet_size> buffer {};

	fflush(stdout);
	auto update_file = FileSystemKit::File("/fs/update.bin");

	// Transfer update file into external flash memory
	while (packet_read != 0) {
		packet_read = update_file.read(buffer.data(), packet_size);
		coreis25lp.write(address, buffer, packet_read);
		address += packet_read;
	}

	// Close the file which also flushes any cached writes
	fflush(stdout);
	update_file.close();

	// Set ready for reboot
	if (int ret = boot_set_pending(0); ret == 0) {
		log_info("> Secondary image pending, reboot to update");
	} else {
		log_error("Failed to set secondary image pending: %d", ret);
	}
}
