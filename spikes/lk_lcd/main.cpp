// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"

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
#include "SDBlockDevice.h"

using namespace leka;

HelloWorld hello;

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

static BufferedSerial serial(USBTX, USBRX, 9600);

constexpr uint8_t buff_size = 128;
char buff[buff_size] {};

Thread screen_thread;

void registerCallbacks(void)
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

int main(void)
{
	auto start = Kernel::Clock::now();

	printf("\nHello, Investigation Day!\n\n");

	rtos::ThisThread::sleep_for(2s);

	corevideo.initialize();
	registerCallbacks();
	initializeSD();

	char filename1[] = "assets/images/Leka/logo.jpg";
	char filename2[] = "assets/images/Leka/emotion-happy.jpg";

	FIL JPEG_File;

	hello.start();
	corevideo.clearScreen();
	rtos::ThisThread::sleep_for(1s);

	uint32_t size = 0;
	Color foreground;
	Color background = CGColor::white;
	for (int i = 1; i <= 20; i++) {
		size	   = sprintf(buff, "Line #%d", i);
		foreground = (i % 2 == 0) ? CGColor::black : CGColor::red;
		corevideo.displayText(buff, size, i, foreground, background);
	}
	rtos::ThisThread::sleep_for(5s);

	size = sprintf(buff,
				   "\tThis sentence is supposed to be on multiple lines because it is too long to be displayed on "
				   "only one line of the screen.");

	corevideo.displayText(buff, size, 10, {0x00, 0x00, 0xFF}, CGColor::white);	 // Write in blue

	rtos::ThisThread::sleep_for(10s);

	while (true) {
		auto t	   = Kernel::Clock::now() - start;
		int length = sprintf(buff, "A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME,
							 hello.world, int(t.count() / 1000));
		serial.write(buff, length);
		rtos::ThisThread::sleep_for(1s);

		if (corefatfs.open(filename1) == FR_OK) {
			corevideo.displayImage(&JPEG_File);
			corevideo.setBrightness(0.2f);

			corevideo.turnOn();

			corefatfs.close();
			rtos::ThisThread::sleep_for(2s);
		}

		if (corefatfs.open(filename2) == FR_OK) {
			corevideo.displayImage(&JPEG_File);
			corevideo.setBrightness(0.9f);

			corefatfs.close();
			rtos::ThisThread::sleep_for(2s);

			corevideo.turnOff();
		}
	}
}
