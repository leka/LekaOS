// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreJPEG.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLL.h"
#include "CoreLTDC.hpp"
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
auto file = FileManagerKit::File {};

VideoKit screen;

auto images = std::to_array({"/fs/images/activity-color_quest.jpg", "/fs/images/color-black.jpg"});
auto videos = std::to_array({"/fs/videos/animation-joy.avi", "/fs/videos/animation-idle.avi"});

extern "C" {
void DSI_IRQHandler(void)
{
	HAL_DSI_IRQHandler(&screen.getDSI().getHandle());
}

void JPEG_IRQHandler(void)
{
	HAL_JPEG_IRQHandler(&screen.getJPEG().getHandle());
}

void DMA2_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(screen.getJPEG().getHandle().hdmain);
}

void DMA2_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(screen.getJPEG().getHandle().hdmaout);
}

void DMA2D_IRQHandler(void)
{
	HAL_DMA2D_IRQHandler(&screen.getDMA2D().getHandle());
}
void LTDC_IRQHandler(void)
{
	HAL_LTDC_IRQHandler(&screen.getLTDC().getHandle());
}
}

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

	auto start = rtos::Kernel::Clock::now();

	logger::init();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	initializeSD();

	screen.initialize();
	screen.setFrameRateLimit(25);

	gfx::Image image1("/fs/images/activity-color_quest.jpg");
	screen.draw(image1);
	screen.display();
	rtos::ThisThread::sleep_for(2s);

	gfx::Image image2("/fs/images/color-black.jpg");
	screen.draw(image2);
	screen.display();
	rtos::ThisThread::sleep_for(2s);

	gfx::Video video_perplex("/fs/videos/animation-perplexity.avi");
	gfx::Video video_joie("/fs/videos/animation-joy.avi");

	gfx::Rectangle progress_bar_bg(0, 460, 800, 20, {190, 250, 230});
	gfx::Rectangle progress_bar(0, 460, 0, 20, {20, 240, 165});

	while (true) {
		while (!video_joie.hasEnded()) {
			progress_bar.width = video_joie.getProgress() * lcd::dimension::width;

			screen.draw(video_joie);
			screen.draw(progress_bar_bg);
			screen.draw(progress_bar);
			screen.display();
		}
		video_joie.restart();

		while (!video_perplex.hasEnded()) {
			int w = video_perplex.getProgress() * lcd::dimension::width;

			screen.draw(video_perplex);
			screen.drawRectangle(0, 460, 800, 20, {250, 190, 230});
			screen.drawRectangle(0, 460, w, 20, {185, 20, 230});
			screen.display();
		}
		video_perplex.restart();
	}

	leka::logger::set_sink_function([](const char *str, size_t size) {
		// corevideo.displayText(str, size, line, foreground, CGColor::white);
	});
}
