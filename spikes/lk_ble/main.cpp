// Leka - LekaOS
// Copyright 2020-2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "BLEKit.h"
#include "BLEServiceBattery.h"
#include "BLEServiceDeviceInformation.h"
#include "BLEServiceMonitoring.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

auto level			 = uint8_t {0};
auto charging_status = bool {false};

auto service_device_information = BLEServiceDeviceInformation {};
auto service_battery			= BLEServiceBattery {};
auto service_monitoring			= BLEServiceMonitoring {};

auto services =
	std::to_array<interface::BLEService *>({&service_device_information, &service_battery, &service_monitoring});

auto blekit = BLEKit {};

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	blekit.setServices(services);
	std::array<uint8_t, 33> serial_number = {"LK-2202-003300294E5350092038384B"};
	service_device_information.setSerialNumber(serial_number);

	blekit.init();

	while (true) {
		log_info("Main thread running...");
		rtos::ThisThread::sleep_for(5s);

		service_battery.setBatteryLevel(level);
		++level;

		charging_status = !charging_status;
		service_monitoring.setChargingStatus(charging_status);
	}
}
