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
#include "UIAnimationKit.h"

using namespace leka;
using namespace std::chrono;

HelloWorld hello;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

CoreSTM32Hal hal;
CoreSDRAM coresdram(hal);
CoreJPEGModeDMA corejpegmode;
CoreJPEG corejpeg(hal, corejpegmode);
CoreDMA2D coredma2d(hal);
CoreLTDC coreltdc(hal);
CoreDSI coredsi(hal, coreltdc);
CoreLCDDriverOTM8009A coreotm(coredsi, PinName::SCREEN_BACKLIGHT_PWM);
CoreLCD corelcd(coreotm);

rtos::Thread animation_thread;
events::EventQueue animation_event_queue;

// animation::BouncingSquare animation_bouncing_square(coregraphics);
// UIAnimationKit animationkit(animation_thread, animation_event_queue);

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n\n");

	auto start = rtos::Kernel::Clock::now();

	rtos::ThisThread::sleep_for(2s);

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
