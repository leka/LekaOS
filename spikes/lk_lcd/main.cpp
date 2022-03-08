// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreSDRAM.hpp"
#include "CoreSTM32Hal.h"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

CoreSTM32Hal hal;
CoreSDRAM coresdram(hal);

VideoKit screen(hal);
VideoKit_DeclareIRQHandlers(screen);

void initializeSD()
{
	constexpr auto default_sd_blockdevice_frequency = uint64_t {25'000'000};

	sd_blockdevice.init();
	sd_blockdevice.frequency(default_sd_blockdevice_frequency);

	fatfs.mount(&sd_blockdevice);
}

void formatTime(char *buffer, int64_t time)
{
	int ds	= (time / 1000);
	int dms = (time / 1000.f - ds) * 100;
	sprintf(buffer, "%02d:%02d,%02d", ds / 60, ds % 60, dms);
}

auto main() -> int
{
	HelloWorld hello;
	hello.start();

	auto start = rtos::Kernel::Clock::now();

	logger::init();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	initializeSD();

	coresdram.initialize();

	screen.initialize();
	screen.setFrameRateLimit(15);

	gfx::Image image1("/fs/images/activity-color_quest.jpg");
	gfx::Image image2("/fs/images/color-black.jpg");

	screen.draw(image1);
	screen.display();
	rtos::ThisThread::sleep_for(2s);

	screen.draw(image2);
	screen.display();
	rtos::ThisThread::sleep_for(2s);

	gfx::Video video_perplex("/fs/videos/animation-perplexity.avi");
	gfx::Video video_joie("/fs/videos/animation-joy.avi");

	auto videos = std::to_array<gfx::Video *, 2>({&video_joie, &video_perplex});

	gfx::Rectangle progress_bar_bg(0, 460, 800, 20, {190, 250, 230});
	gfx::Rectangle progress_bar(0, 460, 0, 20, {20, 240, 165});

	char buff[128];

	while (true) {
		for (auto *video_ptr: videos) {
			auto &video = *video_ptr;
			video.restart();
			while (!video.hasEnded()) {
				screen.draw(video);

				video.nextFrame();

				formatTime(buff, video.getTime());
				screen.drawText(buff, 20, 460, {250, 60, 150});

				screen.display();
			}
		}
	}
}
