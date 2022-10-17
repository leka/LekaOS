// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceFileExchange.h"
#include "BLEServiceMonitoring.h"
#include "BLEServiceUpdate.h"

#include "CircularQueue.h"
#include "CoreEventQueue.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "FileReception.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

auto level			 = uint8_t {0};
auto charging_status = bool {false};

auto service_device_information = BLEServiceDeviceInformation {};
auto service_battery			= BLEServiceBattery {};
auto service_monitoring			= BLEServiceMonitoring {};
auto service_file_exchange		= BLEServiceFileExchange {};
auto service_update				= BLEServiceUpdate {};

auto services = std::to_array<interface::BLEService *>(
	{&service_device_information, &service_battery, &service_monitoring, &service_update, &service_file_exchange});

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
	std::array<uint8_t, 33> serial_number = {"LK-2202-003300294E5350092038384B"};
	service_device_information.setSerialNumber(serial_number);

	auto os_version = Version {123, 234, 45678};
	service_device_information.setOSVersion(os_version);

	blekit.onConnectionCallback([] { log_info("Connected !"); });
	blekit.onDisconnectionCallback([] { log_info("Disconnected !"); });

	blekit.init();

	initializeSD();

	service_file_exchange.onFilePathReceived(
		[](std::span<const char> path) { file_reception_handler.setFilePath(path.data()); });

	service_file_exchange.onFileDataReceived(
		[](std::span<const uint8_t> buffer) { file_reception_handler.onPacketReceived(buffer); });

	while (true) {
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(5s);

		service_battery.setBatteryLevel(level);
		++level;

		charging_status = !charging_status;
		service_monitoring.setChargingStatus(charging_status);

		log_info("Screensaver enable: %d", service_monitoring.isScreensaverEnable());

		auto version = service_update.getVersion();
		log_info("Requested version: %d.%d.%d", version.major, version.minor, version.revision);

		auto advertising_data		 = blekit.getAdvertisingData();
		advertising_data.name		 = "NewLeka";
		advertising_data.battery	 = level;
		advertising_data.is_charging = charging_status;
		blekit.setAdvertisingData(advertising_data);
	}
}
