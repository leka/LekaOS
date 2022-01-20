// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <memory>

#include "platform/Callback.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFatFs.h"
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
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
CoreFatFs corefatfs;

CoreLL corell;
CGPixel pixel(corell);
CoreSTM32Hal hal;
CoreSDRAM coresdram(hal);
CoreDMA2D coredma2d(hal);
CoreDSI coredsi(hal);
CoreLTDC coreltdc(hal, coredsi);
CoreGraphics coregraphics(coredma2d);
CoreFont corefont(pixel);
CoreLCDDriverOTM8009A coreotm(coredsi, PinName::SCREEN_BACKLIGHT_PWM);
CoreLCD corelcd(coreotm);
CoreJPEG corejpeg(hal, coredma2d, corefatfs);
CoreVideo corevideo(hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg);

const auto filename1 = std::array<char, 32> {"assets/images/Leka/logo.jpg"};
const auto filename2 = std::array<char, 38> {"assets/images/Leka/emotion-happy.jpg"};

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
	registerCallbacks();

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
		rtos::ThisThread::sleep_for(1s);
	}

	rtos::ThisThread::sleep_for(5s);

	leka::logger::set_sink_function([](const char *str, size_t size) {
		corevideo.displayText(str, size, 10, {0x00, 0x00, 0xFF}, CGColor::white);	// write in blue
	});

	log_info(
		"This sentence is supposed to be on multiple lines because it is too long to be displayed on "
		"only one line of the screen.");

	rtos::ThisThread::sleep_for(10s);

	leka::logger::set_sink_function(logger::internal::default_sink_function);

	auto JPEG_File = std::make_unique<FIL>();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %is", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		rtos::ThisThread::sleep_for(1s);

		if (corefatfs.open(filename1.data()) == FR_OK) {
			corevideo.displayImage(JPEG_File.get());
			corevideo.setBrightness(0.2F);

			corevideo.turnOn();

			corefatfs.close();
			rtos::ThisThread::sleep_for(2s);
		}

		if (corefatfs.open(filename2.data()) == FR_OK) {
			corevideo.displayImage(JPEG_File.get());
			corevideo.setBrightness(0.9F);

			corefatfs.close();
			rtos::ThisThread::sleep_for(2s);

			corevideo.turnOff();
		}
	}
}
