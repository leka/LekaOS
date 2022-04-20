// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceFileReception.h"
#include "BLEServiceMonitoring.h"
#include "BLEServiceUpdate.h"

#include "CircularQueue.h"
#include "CoreEventQueue.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

auto level			 = uint8_t {0};
auto charging_status = bool {false};

auto file_reception_circular_queue = CircularQueue<uint8_t, 1024> {};
auto write_buffer				   = std::array<uint8_t, 128> {};
auto event_queue				   = CoreEventQueue {};

auto service_device_information = BLEServiceDeviceInformation {};
auto service_battery			= BLEServiceBattery {};
auto service_monitoring			= BLEServiceMonitoring {};
auto service_file_reception		= BLEServiceFileReception {};
auto service_update				= BLEServiceUpdate {};

auto services = std::to_array<interface::BLEService *>(
	{&service_device_information, &service_battery, &service_monitoring, &service_update, &service_file_reception});

auto blekit = BLEKit {};

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
FileManagerKit::File reception_file {};

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

	blekit.init();

	initializeSD();

	event_queue.dispatch_forever();

	if (reception_file.open(service_file_reception.getFilePath().data(), "w")) {
		reception_file.close();
	}

	auto on_file_data_write = []() {
		auto path = service_file_reception.getFilePath();

		if (reception_file.open(path.data(), "a")) {
			write_buffer.fill('\0');
			auto data_read = file_reception_circular_queue.pop(write_buffer.data(), std::size(write_buffer));
			reception_file.write(write_buffer.data(), data_read);
			reception_file.close();
		}
	};

	auto on_file_data_received = [&on_file_data_write](std::span<uint8_t> buffer) {
		file_reception_circular_queue.push(buffer.data(), std::size(buffer));

		event_queue.call(on_file_data_write);
	};
	service_file_reception.onFileDataReceived(on_file_data_received);

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
	}
}
