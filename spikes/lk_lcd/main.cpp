// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreDMA2D.h"
#include "CoreDSI.h"
#include "CoreJPEG.h"
#include "CoreLCD.h"
#include "CoreLCDDriverOTM8009A.h"
#include "CoreLTDC.h"
#include "CoreSDRAM.h"
#include "CoreVideo.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LKCoreFatFs.h"
#include "LKCoreLL.h"
#include "LKCoreSTM32Hal.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
LKCoreFatFs file;

/*
LKCoreSTM32Hal hal;
CoreSDRAM coresdram(hal);

// screen + dsi + ltdc
CoreLTDC coreltdc(hal);
CoreDSI coredsi(hal, coreltdc);
CoreLCDDriverOTM8009A coreotm(coredsi, PinName::SCREEN_BACKLIGHT_PWM);
CoreLCD corelcd(coreotm);

// peripherals
CoreDMA2D coredma2d(hal);
CoreJPEG corejpeg(hal, std::make_unique<CoreJPEGDMAMode>());

LKCoreVideo corevideo(hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, corejpeg);
*/
VideoKit screen;

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

	auto start = rtos::Kernel::Clock::now();

	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	initializeSD();

	// corevideo.initialize();
	screen.initialize();
	screen.setFrameRateLimit(25);

	gfx::Image image1(images[0]);
	gfx::Video video_perplex(videos[0]);
	gfx::Video video_joie(videos[1]);

	std::array<gfx::Video *, 2> videos_array = {&video_joie, &video_perplex};

	int video_index = 0;
	int w			= 0;
	while (true) {
		auto *video = videos_array[video_index];
		if (video->hasEnded()) {
			video->restart();
			video_index = (video_index + 1) % videos_array.size();
			video		= videos_array[video_index];
		}

		w = video->getProgress() * lcd::dimension.width;

		screen.draw(*video);
		screen.drawRectangle({800, 20, {250, 190, 230}}, 0, 460);
		screen.drawRectangle({w, 20, {185, 20, 230}}, 0, 460);
		screen.display();
	}

	// corevideo.clearScreen();

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

	// coredsi.refresh();

	rtos::ThisThread::sleep_for(1s);
	*/

	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	// HAL_LTDC_ProgramLineEvent(&coreltdc.getHandle(), 0);
	// corevideo.setBrightness(0.6f);
	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %is", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		// while (true) {
		for (const auto &image_name: images) {
			if (file.open(image_name) == FR_OK) {
				// corevideo.displayImage(file);
				// corevideo.display();
				// corevideo.turnOn();
				file.close();
				rtos::ThisThread::sleep_for(1s);
			}
		}
		//}

		for (const auto &video_name: videos) {
			if (file.open(video_name) == FR_OK) {
				// corevideo.displayVideo(file);
				file.close();
				rtos::ThisThread::sleep_for(500ms);
			}
		}

		// corevideo.turnOff();
		rtos::ThisThread::sleep_for(500ms);
	}
}
