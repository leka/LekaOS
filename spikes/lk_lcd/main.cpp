// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "rtos/ThisThread.h"

#include "Assets.h"
#include "CoreSDRAM.hpp"
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

auto hal	   = CoreSTM32Hal {};
auto coresdram = CoreSDRAM {hal};

auto screen = VideoKit {hal};
VideoKit_DeclareIRQHandlers(screen);

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

	coresdram.initialize();

	screen.initialize();
	screen.setFrameRateLimit(30);

	for (auto &name: image_names) {
		auto image = gfx::Image {name};

		screen.draw(image);
		screen.display();

		rtos::ThisThread::sleep_for(250ms);
	}

	while (true) {
		for (auto &name: video_names) {
			auto video = gfx::Video {name};

			while (!video.hasEnded()) {
				screen.draw(video);
				video.nextFrame();
				screen.display();
			}
		}
	}
}
