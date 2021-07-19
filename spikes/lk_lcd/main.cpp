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

std::vector<const char *> images = {"assets/images/Leka/logo.jpg", "assets/images/Leka/image.jpg"};

std::vector<const char *> videos = {
	//"assets/video/20fps.avi",
	"assets/video/20fps_low10.avi",
	//"assets/video/20fps_s700.avi",
	//"assets/video/20fps_s600.avi",
	//"assets/video/20fps_s500.avi",
	//"assets/video/20fps_s400.avi",
	//"assets/video/BirdsAndFeeder.avi",
	//"assets/video/20fps_s300.avi",
	//"assets/video/20fps_s200.avi",
	//"assets/video/20fps_s100.avi"
	"assets/video/Perplex_10.avi",
};

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
	sd_blockdevice.init();
	sd_blockdevice.frequency(25'000'000);

	fatfs.mount(&sd_blockdevice);
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
	screen.setFrameRateLimit(25);

	gfx::Image image1("assets/images/Leka/logo.jpg");
	gfx::Image image2("assets/images/Leka/image.jpg");

	screen.draw(image1);
	screen.display();
	rtos::ThisThread::sleep_for(2s);

	screen.draw(image2);
	screen.display();
	rtos::ThisThread::sleep_for(2s);

	gfx::Video video_birds("assets/video/BirdsAndFeeder_low.avi");
	gfx::Video video_joie("assets/video/20fps_low10.avi");

	gfx::Rectangle progress_bar_bg(0, 460, 800, 20, {190, 250, 230});
	gfx::Rectangle progress_bar(0, 460, 0, 20, {20, 240, 165});

	while (true) {
		while (!video_joie.hasEnded()) {
			screen.draw(video_joie);

			video_joie.nextFrame();
			progress_bar.width = video_joie.getProgress() * lcd::dimension.width;

			screen.draw(progress_bar_bg);
			screen.draw(progress_bar);

			screen.display();
		}
		video_joie.restart();

		while (!video_birds.hasEnded()) {
			screen.draw(video_birds);

			video_birds.nextFrame();
			int w = video_birds.getProgress() * lcd::dimension.width;

			screen.drawRectangle(0, 460, 800, 20, {250, 190, 230});
			screen.drawRectangle(0, 460, w, 20, {185, 20, 230});

			screen.display();
		}
		video_birds.restart();
	}

	leka::logger::set_print_function([](const char *str, size_t size) {
		// corevideo.displayText(str, size, line, foreground, CGColor::white);
	});

	/*
	for (int i = 1; i <= 10; i++) {
		foreground = (i % 2 == 0) ? CGColor::black : CGColor::pure_red;
		line	   = i * 2;
		log_info("Line #%i", i);
		rtos::ThisThread::sleep_for(200ms);
	}

	leka::logger::set_print_function([](const char *str, size_t size) {
		// corevideo.displayText(str, size, 10, {0x00, 0x00, 0xFF}, CGColor::white);	// write in blue
	});

	log_info(
		"This sentence is supposed to be on multiple lines because it is too long to be displayed on "
		"only one line of the screen.");

	rtos::ThisThread::sleep_for(1s);
	*/
}
