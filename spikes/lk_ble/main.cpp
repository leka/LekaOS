// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceFileExchange.h"

#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "FileReception.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

auto service_device_information = BLEServiceDeviceInformation {};
auto service_file_exchange		= BLEServiceFileExchange {};

auto services = std::to_array<interface::BLEService *>({&service_device_information, &service_file_exchange});

auto blekit = BLEKit {};

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
auto file_reception_handler = FileReception {};

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

	blekit.setServices(services);

	blekit.onConnectionCallback([] { log_info("Connected !"); });
	blekit.onDisconnectionCallback([] { log_info("Disconnected !"); });

	blekit.init();

	initializeSD();

	service_file_exchange.onSetFileExchangeState(
		[](bool new_state) { log_info("New FileExchange state is: %s", (new_state ? "on" : "off")); });

	service_file_exchange.onFilePathReceived(
		[](std::span<const char> path) { file_reception_handler.setFilePath(path.data()); });

	service_file_exchange.onClearFileRequested([] { file_reception_handler.clearFile(); });

	service_file_exchange.onFileDataReceived(
		[](std::span<const uint8_t> buffer) { file_reception_handler.onPacketReceived(buffer); });

	while (true) {
		log_info("still alive");
		rtos::ThisThread::sleep_for(1s);
	}
}
