// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "events/EventQueue.h"
#include "rtos/Kernel.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "BouncingSquare.h"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreJPEG.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLTDC.hpp"
#include "CoreSDRAM.hpp"
#include "CoreVideo.hpp"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "LKAnimationKit.h"
#include "LKCoreFatFs.h"
#include "LKCoreLL.h"
#include "LKCoreSTM32Hal.h"
#include "LogKit.h"
#include "SDBlockDevice.h"

using namespace leka;
using namespace std::chrono;

HelloWorld hello;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");
LKCoreFatFs corefatfs;

LKCoreLL corell;
CGPixel pixel(corell);
LKCoreSTM32Hal hal;
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

rtos::Thread animation_thread;
events::EventQueue animation_event_queue;

animation::BouncingSquare animation_bouncing_square(coregraphics);
LKAnimationKit animationkit(animation_thread, animation_event_queue);

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_trace_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

	corevideo.initialize();

	hello.start();

	while (true) {
		auto t = rtos::Kernel::Clock::now() - start;
		log_info("A message from your board %s --> \"%s\" at %i s\n", MBED_CONF_APP_TARGET_NAME, hello.world,
				 int(t.count() / 1000));

		animationkit.start(animation_bouncing_square);
		rtos::ThisThread::sleep_for(5s);

		animationkit.stop();
		rtos::ThisThread::sleep_for(1s);
	}
}
