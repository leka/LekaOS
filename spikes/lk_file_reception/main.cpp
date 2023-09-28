// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "CoreNetwork.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "WebKit.h"

using namespace leka;
using namespace std::chrono;

auto web_access		 = CoreNetwork {};
auto web_file_handle = FileManagerKit::File {};
auto web_kit		 = WebKit(web_access, web_file_handle);

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

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

	auto start = rtos::Kernel::Clock::now();

	auto hello = HelloWorld();

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	initializeSD();

	rtos::ThisThread::sleep_for(1s);

	if (auto is_connected = web_kit.connect("USER_SSID", "USER_PASS"); is_connected) {
		// clang-format off
		auto certificates_path_list = std::to_array(
			{
			"fs/usr/certs/ca/DigiCert_TLS_Hybrid_ECC_SHA384_2020_CA1-07F2F35C87A877AF7AEFE947993525BD.txt",
			"fs/usr/certs/ca/DigiCert_TLS_RSA_SHA256_2020_CA1-06D8D904D5584346F68A2FA754227EC4.txt",
			 });
		// clang-format on

		web_kit.setCertificateStore(certificates_path_list);

		log_info("Start downloading file via wifi...");

		auto downloadable_file = WebKit::DownloadableFile {
			.url	 = "https://github.com/leka/LekaOS/releases/download/v1.4.0/LekaOS-1.4.0+1678882778.bin",
			.to_path = "/fs/usr/os/LekaOS-1.4.0.bin"};
		auto file_downloaded = web_kit.downloadFile(downloadable_file);

		if (file_downloaded) {
			log_info("Success to download file");
		} else {
			log_info("Fail to download file");
		}
	}

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));
		rtos::ThisThread::sleep_for(10s);
	}
}
