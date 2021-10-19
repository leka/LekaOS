// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreQSPI.h"
#include "FATFileSystem.h"
#include "FileSystemKit.h"
#include "FirmwareKit.h"
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
auto firmwarekit	   = FirmwareKit(coreis25lp);

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

	std::string path = "/fs/os/";
	firmwarekit.setDefaultPath(path);

	// Load file
	auto version = FirmwareVersion {.major = 1, .minor = 2, .revision = 3};
	if (auto is_loaded = firmwarekit.loadUpdate(version); is_loaded) {
		log_info("New update is loaded in external flash");
	}

	// Set ready for reboot
	if (int ret = boot_set_pending(0); ret == 0) {
		log_info("> Secondary image pending, reboot to update");
	} else {
		log_error("Failed to set secondary image pending: %d", ret);
	}
}
