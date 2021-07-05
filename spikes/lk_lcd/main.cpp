// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <memory>

#include "drivers/BufferedSerial.h"
#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LKCoreDMA2D.h"
#include "LKCoreDSI.h"
#include "LKCoreFatFs.h"
#include "LKCoreFont.h"
#include "LKCoreGraphics.h"
#include "LKCoreJPEG.h"
#include "LKCoreLCD.h"
#include "LKCoreLCDDriverOTM8009A.h"
#include "LKCoreLL.h"
#include "LKCoreLTDC.h"
#include "LKCoreSDRAM.h"
#include "LKCoreSTM32Hal.h"
#include "LKCoreVideo.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
LKCoreFatFs file;

LKCoreLL corell;
CGPixel pixel(corell);
LKCoreSTM32Hal hal;
LKCoreSDRAM coresdram(hal);
LKCoreDMA2D coredma2d(hal);
LKCoreDSI coredsi(hal);
LKCoreLTDC coreltdc(hal, coredsi);
LKCoreGraphics coregraphics(coredma2d);
LKCoreFont corefont(pixel);
LKCoreLCDDriverOTM8009A coreotm(coredsi, PinName::SCREEN_BACKLIGHT_PWM);
LKCoreLCD corelcd(coreotm);
LKCoreJPEG corejpeg(hal, std::make_unique<LKCoreJPEGDMAMode>());
LKCoreVideo corevideo(hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg);

std::vector<const char*> images = {"assets/images/Leka/logo.jpg", "assets/images/Leka/emotion-happy.jpg"};

extern "C" {
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
	sd_blockdevice.init();
	sd_blockdevice.frequency(25'000'000);

	fatfs.mount(&sd_blockdevice);
}

auto main() -> int
{
	auto start = rtos::Kernel::Clock::now();

	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	corevideo.initialize();

	initializeSD();

	HelloWorld hello;
	hello.start();

	corevideo.clearScreen();

	static auto line = 1;
	static CGColor foreground;

	leka::logger::set_print_function(
		[](const char *str, size_t size) { corevideo.displayText(str, size, line, foreground, CGColor::white); });

	for (int i = 1; i <= 10; i++) {
		foreground = (i % 2 == 0) ? CGColor::black : CGColor::pure_red;
		line	   = i * 2;
		log_info("Line #%i", i);
		rtos::ThisThread::sleep_for(200ms);
	}

	leka::logger::set_print_function([](const char *str, size_t size) {
		corevideo.displayText(str, size, 10, {0x00, 0x00, 0xFF}, CGColor::white);	// write in blue
	});

	log_info(
		"This sentence is supposed to be on multiple lines because it is too long to be displayed on "
		"only one line of the screen.");

	rtos::ThisThread::sleep_for(1s);

	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %is", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		corevideo.setBrightness(0.6f);

		for (const auto &image_name: images) {
			if (file.open(image_name) == FR_OK) {
				corevideo.displayImage(file);
				corevideo.turnOn();
				file.close();
				rtos::ThisThread::sleep_for(2s);
			}
		}

		corevideo.turnOff();
	}
}
