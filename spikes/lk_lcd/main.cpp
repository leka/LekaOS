// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <memory>

#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "./Images.h"
#include "./Videos.h"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreEventFlags.h"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreJPEGModePolling.hpp"
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
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

namespace display::internal {

auto event_flags = CoreEventFlags {};

auto corell		   = CoreLL {};
auto pixel		   = CGPixel {corell};
auto hal		   = CoreSTM32Hal {};
auto coresdram	   = CoreSDRAM {hal};
auto coredma2d	   = CoreDMA2D {hal};
auto coredsi	   = CoreDSI {hal};
auto coreltdc	   = CoreLTDC {hal};
auto coregraphics  = CoreGraphics {coredma2d};
auto corefont	   = CoreFont {pixel};
auto coreotm	   = CoreLCDDriverOTM8009A {coredsi, PinName::SCREEN_BACKLIGHT_PWM};
auto corelcd	   = CoreLCD {coreotm};
auto _corejpegmode = CoreJPEGModeDMA {hal};
auto corejpeg	   = CoreJPEG {hal, _corejpegmode};

extern "C" auto corevideo =
	CoreVideo {hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg};

}	// namespace display::internal

auto videokit = VideoKit {display::internal::event_flags, display::internal::corevideo};

auto file = FileManagerKit::File {};

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

	videokit.initializeScreen();

	initializeSD();

	HelloWorld hello;
	hello.start();

	display::internal::corevideo.clearScreen();
	rtos::ThisThread::sleep_for(1s);

	static auto line = 1;
	static CGColor foreground;
	static CGColor background = CGColor::white;

	leka::logger::set_sink_function([](const char *str, std::size_t size) {
		display::internal::corevideo.displayText(str, size, line, foreground, background);
	});

	for (int i = 1; i <= 10; i++) {
		foreground = (i % 2 == 0) ? CGColor::black : CGColor::pure_red;
		line	   = i * 2;
		log_info("Line #%i", i);
		rtos::ThisThread::sleep_for(100ms);
	}

	rtos::ThisThread::sleep_for(500ms);

	leka::logger::set_sink_function([](const char *str, std::size_t size) {
		display::internal::corevideo.displayText(str, size, 10, {0x00, 0x00, 0xFF}, CGColor::white);   // write in blue
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

		rtos::ThisThread::sleep_for(1s);

		display::internal::corelcd.setBrightness(0.9F);
		display::internal::corelcd.turnOn();

		for (const auto &image_name: images) {
			videokit.displayImage(image_name);
			rtos::ThisThread::sleep_for(1s);
		}

		auto on_video_ended_callback = [] { log_debug("End of the video"); };

		for (const auto &video_name: videos) {
			videokit.playVideoOnce(video_name, on_video_ended_callback);
			rtos::ThisThread::sleep_for(1s);
		}

		display::internal::corelcd.turnOff();
	}
}
