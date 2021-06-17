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
LKCoreFatFs corefatfs;

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
LKCoreJPEG corejpeg(hal, coredma2d, corefatfs);
LKCoreVideo corevideo(hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg);

const char filename1[] = "assets/images/Leka/logo.jpg";
const char filename2[] = "assets/images/Leka/emotion-happy.jpg";
const char filename3[] = "assets/video/BirdsAndFeeder.avi";

void registerCallbacks()
{
	HAL_JPEG_RegisterInfoReadyCallback(
		corejpeg.getHandlePointer(),
		[](JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) { corejpeg.onInfoReadyCallback(hjpeg, info); });

	HAL_JPEG_RegisterGetDataCallback(corejpeg.getHandlePointer(), [](JPEG_HandleTypeDef *hjpeg, uint32_t size) {
		corejpeg.onDataAvailableCallback(hjpeg, size);
	});

	HAL_JPEG_RegisterDataReadyCallback(corejpeg.getHandlePointer(),
									   [](JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t size) {
										   corejpeg.onDataReadyCallback(hjpeg, pDataOut, size);
									   });

	HAL_JPEG_RegisterCallback(corejpeg.getHandlePointer(), HAL_JPEG_DECODE_CPLT_CB_ID,
							  [](JPEG_HandleTypeDef *hjpeg) { corejpeg.onDecodeCompleteCallback(hjpeg); });

	HAL_JPEG_RegisterCallback(corejpeg.getHandlePointer(), HAL_JPEG_ERROR_CB_ID,
							  [](JPEG_HandleTypeDef *hjpeg) { corejpeg.onErrorCallback(hjpeg); });
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

	rtos::ThisThread::sleep_for(2s);

	corevideo.initialize();
	registerCallbacks();

	initializeSD();

	HelloWorld hello;
	hello.start();

	corevideo.clearScreen();
	rtos::ThisThread::sleep_for(1s);

	static auto line = 1;
	static CGColor foreground;
	static CGColor background = CGColor::white;

	leka::logger::set_print_function(
		[](const char *str, size_t size) { corevideo.displayText(str, size, line, foreground, background); });

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

	rtos::ThisThread::sleep_for(2s);

	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	auto JPEG_File = std::make_unique<FIL>();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %is", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);

		corevideo.turnOn();
		if (corefatfs.open(filename1.data()) == FR_OK) {
			log_info("Displaying image 1 : %s", filename1.data());
			corevideo.displayImage(JPEG_File.get());
			corevideo.setBrightness(0.2f);


			corefatfs.close();
			rtos::ThisThread::sleep_for(2s);
		}

		if (corefatfs.open(filename2.data()) == FR_OK) {
			log_info("Displaying image 2 : %s", filename2.data());
			corevideo.displayImage(JPEG_File.get());
			corevideo.setBrightness(0.9f);

			corefatfs.close();
			rtos::ThisThread::sleep_for(2s);

		}

		if (corefatfs.open(filename3) == FR_OK) {
			log_info("Displaying video : %s", filename3);
			corejpeg.playVideo();
			corefatfs.close();
		}
		corevideo.turnOff();
		rtos::ThisThread::sleep_for(1s);
	}
}
