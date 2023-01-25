// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "PinNames.h"

#include "rtos/ThisThread.h"

#include "ComUtils.h"
#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreJPEGModePolling.hpp"
#include "CoreLCD.hpp"
#include "CoreLCDDriverOTM8009A.hpp"
#include "CoreLL.h"
#include "CoreLTDC.hpp"
#include "CorePwm.h"
#include "CoreSDRAM.hpp"
#include "CoreSTM32Hal.h"
#include "CoreVideo.hpp"
#include "EventLoopKit.h"
#include "FATFileSystem.h"
#include "HelloWorld.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

//
// MARK: - Global definitions
//

using namespace leka;
using namespace std::chrono;

namespace {

namespace sd {

	namespace internal {

		auto bd = SDBlockDevice {SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK};
		auto fs = FATFileSystem {"fs"};

		constexpr auto default_frequency = uint64_t {25'000'000};

	}	// namespace internal

	void init()
	{
		internal::bd.init();
		internal::bd.frequency(internal::default_frequency);
		internal::fs.mount(&internal::bd);
	}

}	// namespace sd

namespace display::internal {

	auto event_loop = EventLoopKit {};

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

auto videokit = VideoKit {display::internal::event_loop, display::internal::corevideo};

}	// namespace

auto hello = HelloWorld {};

auto com_utils_flag = rtos::EventFlags {};
auto com			= ComUtils {com_utils_flag};

auto main() -> int
{
	sd::init();

	display::internal::corelcd.turnOn();
	videokit.initializeScreen();

	rtos::ThisThread::sleep_for(1s);

	hello.start();

	while (true) {
		com_utils_flag.wait_any(ComUtils::flags::data_available);

		auto path = com.getPath();

		if (path.string().ends_with("need_reboot")) {
			while (true) {
				// ? hang program to reboot
			}
		}

		if (auto is_missing = !(std::filesystem::exists(path)); is_missing) {
			com.write("NOK_MISSING:" + path.string());
			continue;
		}

		if (auto is_empty = std::filesystem::is_empty(path); is_empty) {
			com.write("NOK_EMPTY:" + path.string());
			continue;
		}

		if (path.string().ends_with(".jpg")) {
			videokit.displayImage(path);
			rtos::ThisThread::sleep_for(100ms);
			com.write("ACK_IMAGE:" + path.string());
			continue;
		}

		if (path.string().ends_with(".avi")) {
			videokit.displayImage("/fs/home/img/system/robot-misc-robot-misc-screen_empty_white.jpg");
			rtos::ThisThread::sleep_for(5ms);
			videokit.playVideoOnce(path);
			rtos::ThisThread::sleep_for(3s);
			videokit.stopVideo();
			rtos::ThisThread::sleep_for(5ms);
			com.write("ACK_VIDEO:" + path.string());
			continue;
		}

		if (auto *file = std::fopen(path.c_str(), "r"); file != nullptr) {
			std::vector<char> buf(4);	// char is trivally copyable

			std::fread(buf.data(), sizeof(buf[0]), buf.size(), file);
			std::fclose(file);

			if (buf.empty()) {
				com.write("NOK_EMPTY:" + path.string());
				continue;
			}

			com.write("ACK_FILE:" + path.string());
			continue;
		}

		com.write("NOK_NOT_OPEN:" + path.string());
	}
}
