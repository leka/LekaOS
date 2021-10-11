// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreQSPI.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "QSPIFBlockDevice.h"
#include "SDBlockDevice.h"
#include "SlicingBlockDevice.h"
#include "bootutil/bootutil.h"

#define TRACE_GROUP "main"
#include "mbed-trace/mbed_trace.h"

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
	static QSPIFBlockDevice _bd;

	static mbed::SlicingBlockDevice sliced_bd(&_bd, 0x0, MCUBOOT_SLOT_SIZE);
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

	// Enable traces from relevant trace groups
	mbed_trace_init();
	mbed_trace_include_filters_set("main,MCUb,BL");

	// Initialization
	hello.start();

	initializeSD();

	coreis25lp.reset();
	coreqspi.setDataTransmissionFormat();
	coreqspi.setFrequency(flash::is25lp016d::max_clock_frequency_in_hz);
	coreis25lp.erase();

	// Open file and initialize tools
	fflush(stdout);
	FILE *f = fopen("/fs/update.bin", "r+");
	log_debug("%s", (!f ? "Fail :(" : "OK"));
	uint32_t address		 = 0x0;
	const size_t packet_size = 0x100;
	std::array<uint8_t, packet_size> buffer {};

	// Transfer update file into external flash memory
	while (!feof(f)) {
		for (uint16_t i = 0; i < packet_size; i++) {
			buffer[i] = fgetc(f);
		}
		coreis25lp.write(address, buffer, packet_size);
		address += packet_size;
	}

	// Close the file which also flushes any cached writes
	log_debug("Closing \"/fs/update.bin\"... ");
	fflush(stdout);
	int err = fclose(f);
	log_debug("%s", (err < 0 ? "Fail :(" : "OK"));
	if (err < 0) {
		log_error("error: %s (%d)\n", strerror(errno), -errno);
	}

	// Set ready for reboot
	int ret = boot_set_pending(0);
	if (ret == 0) {
		log_info("> Secondary image pending, reboot to update");
	} else {
		log_error("Failed to set secondary image pending: %d", ret);
	}
}
