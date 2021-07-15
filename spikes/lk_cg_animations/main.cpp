// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "events/EventQueue.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "BouncingSquare.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LKAnimationKit.h"
#include "CoreDMA2D.h"
#include "CoreDSI.h"
#include "LKCoreFatFs.h"
#include "CoreJPEG.h"
#include "CoreLCD.h"
#include "CoreLCDDriverOTM8009A.h"
#include "LKCoreLL.h"
#include "CoreLTDC.h"
#include "CoreSDRAM.h"
#include "LKCoreSTM32Hal.h"
#include "CoreVideo.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

HelloWorld hello;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
LKCoreFatFs corefatfs;

LKCoreSTM32Hal hal;
CoreSDRAM coresdram(hal);
CoreDMA2D coredma2d(hal);
CoreLTDC coreltdc(hal);
CoreDSI coredsi(hal, coreltdc);
CoreLCDDriverOTM8009A coreotm(coredsi, PinName::SCREEN_BACKLIGHT_PWM);
CoreLCD corelcd(coreotm);
CoreJPEG corejpeg(hal, std::make_unique<CoreJPEGDMAMode>());
CoreVideo corevideo(hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, corejpeg);

rtos::Thread animation_thread;
events::EventQueue animation_event_queue;

// animation::BouncingSquare animation_bouncing_square(coregraphics);
// LKAnimationKit animationkit(animation_thread, animation_event_queue);

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	corevideo.initialize();

	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		// animationkit.start(animation_bouncing_square);
		rtos::ThisThread::sleep_for(5s);

		// animationkit.stop();
		rtos::ThisThread::sleep_for(1s);
	}
}
