// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreSDRAM.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LKCoreFatFs.h"
#include "LKCoreSTM32Hal.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

LKCoreSTM32Hal hal;
CoreSDRAM coresdram(hal);

VideoKit screen(hal);
VideoKit_DeclareIRQHandlers(screen);

void initializeSD()
{
	sd_blockdevice.init();
	sd_blockdevice.frequency(25'000'000);

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

	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	initializeSD();

	coresdram.initialize();

	screen.initialize();
	screen.setFrameRateLimit(15);

	gfx::Image image1("/fs/assets/images/Leka/logo.jpg");
	gfx::Image image2("/fs/assets/images/Leka/image.jpg");

	screen.draw(image1);
	screen.display();
	rtos::ThisThread::sleep_for(2s);

	screen.draw(image2);
	screen.display();
	rtos::ThisThread::sleep_for(2s);

	gfx::Video video_joie("/fs/assets/video/joie_15fps_10.avi");
	gfx::Video video_perplex("/fs/assets/video/perplex_15fps_10.avi");
	gfx::Video video_birds("/fs/assets/video/BirdsAndFeeder_15fps_10.avi");

	std::array<gfx::Video *, 3> videos = {&video_joie, &video_perplex, &video_birds};

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
				progress_bar.width = video.getProgress() * lcd::dimension.width;

				screen.draw(progress_bar_bg);
				screen.draw(progress_bar);

				formatTime(buff, video.getTime());
				screen.drawText(buff, 20, 460, {250, 60, 150});

				screen.display();
			}
		}
	}
}
