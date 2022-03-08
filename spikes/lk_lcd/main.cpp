// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "Assets.h"
#include "CoreSTM32Hal.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono_literals;

auto sd_blockdevice = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
auto fatfs			= FATFileSystem {"fs"};

auto file = FileManagerKit::File {};

auto image_names_small = std::to_array({"/fs/assets/images/Leka/logo.jpg", "/fs/assets/images/Leka/emotion-happy.jpg"});

auto hal	  = CoreSTM32Hal {};
auto videokit = VideoKit {hal};

VideoKit_DeclareIRQHandlers(videokit);

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	HelloWorld hello;
	hello.start();

	logger::init();

	log_info("Hello, World!\n\n");

	initializeSD();

	videokit.initializeScreen();

	for (auto &name: image_names) {
		log_info("display %s", name);
		videokit.displayImage(name);
		rtos::ThisThread::sleep_for(1s);
	}

	while (true) {
		for (auto &name: video_names) {
			log_info("display %s", name);
			videokit.playVideo(name);
			rtos::ThisThread::sleep_for(3s);
		}
	}
}
