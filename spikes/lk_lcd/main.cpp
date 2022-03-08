// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <memory>

#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreJPEG.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLL.h"
#include "CoreLTDC.hpp"
#include "CoreSDRAM.hpp"
#include "CoreSTM32Hal.h"
#include "CoreVideo.hpp"
#include "FATFileSystem.h"
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
auto file = FileManagerKit::File {};

CoreLL corell;
CGPixel pixel(corell);
CoreSTM32Hal hal;
CoreSDRAM coresdram(hal);
CoreDMA2D coredma2d(hal);
CoreDSI coredsi(hal);
CoreLTDC coreltdc(hal);
CoreGraphics coregraphics(coredma2d);
CoreFont corefont(pixel);
CoreLCDDriverOTM8009A coreotm(coredsi, PinName::SCREEN_BACKLIGHT_PWM);
CoreLCD corelcd(coreotm);
CoreJPEG corejpeg(hal, std::make_unique<CoreJPEGDMAMode>());
CoreVideo corevideo(hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg);

auto images = std::to_array({"/fs/images/activity-color_quest.jpg", "/fs/images/color-black.jpg"});
auto videos = std::to_array({"/fs/videos/animation-joy.avi", "/fs/videos/animation-idle.avi"});

extern "C" {
void DMA2D_IRQHandler(void)
{
	HAL_DMA2D_IRQHandler(&coredma2d.getHandle());
}

void LTDC_IRQHandler(void)
{
	HAL_LTDC_IRQHandler(&coreltdc.getHandle());
}

void DSI_IRQHandler(void)
{
	HAL_DSI_IRQHandler(&coredsi.getHandle());
}

void JPEG_IRQHandler(void)
{
	HAL_JPEG_IRQHandler(&corejpeg.getHandle());
}

void DMA2_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(corejpeg.getHandle().hdmain);
}

void DMA2_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(corejpeg.getHandle().hdmaout);
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
	auto start = rtos::Kernel::Clock::now();

	logger::init();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	corevideo.initialize();

	initializeSD();

	HelloWorld hello;
	hello.start();

	corevideo.clearScreen();
	rtos::ThisThread::sleep_for(1s);

	static auto line = 1;
	static CGColor foreground;
	static CGColor background = CGColor::white;

	leka::logger::set_sink_function(
		[](const char *str, size_t size) { corevideo.displayText(str, size, line, foreground, background); });

	for (int i = 1; i <= 10; i++) {
		foreground = (i % 2 == 0) ? CGColor::black : CGColor::pure_red;
		line	   = i * 2;
		log_info("Line #%i", i);
		rtos::ThisThread::sleep_for(100ms);
	}

	rtos::ThisThread::sleep_for(500ms);

	leka::logger::set_sink_function([](const char *str, size_t size) {
		corevideo.displayText(str, size, 10, {0x00, 0x00, 0xFF}, CGColor::white);	// write in blue
	});

	log_info(
		"This sentence is supposed to be on multiple lines because it is too long to be displayed on "
		"only one line of the screen.");

	rtos::ThisThread::sleep_for(1s);

	leka::logger::set_sink_function(logger::internal::default_sink_function);

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %is", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		for (const auto &image_name: images) {
			if (file.open(image_name)) {
				log_info("open");
				corevideo.displayImage(file);
				corevideo.turnOn();
				file.close();
				rtos::ThisThread::sleep_for(2s);
			}
		}

		for (const auto &video_name: videos) {
			if (file.open(video_name)) {
				corevideo.displayVideo(file);
				file.close();
				rtos::ThisThread::sleep_for(2s);
			}
		}

		corevideo.turnOff();
	}
}
