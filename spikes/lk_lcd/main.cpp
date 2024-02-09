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
#include "FileManagerKit.h"
#include "HelloWorld.h"
#include "LogKit.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;
using namespace std::chrono;

#include "mbed_stats.h"

#include "drivers/Watchdog.h"
namespace {

namespace watchdog {

	namespace internal {

		auto &instance		   = mbed::Watchdog::get_instance();
		constexpr auto timeout = 30000ms;
		auto thread			   = rtos::Thread {osPriorityLow};

		namespace stats {

			auto cpu   = mbed_stats_cpu_t {};
			auto stack = mbed_stats_stack_t {};
			auto heap  = mbed_stats_heap_t {};

		}	// namespace stats

		__attribute__((noreturn)) void watchdog_kick()
		{
			static auto kick_count = uint32_t {0};

			static auto start = rtos::Kernel::Clock::now();
			static auto stop  = rtos::Kernel::Clock::now();
			static auto delta = static_cast<int>((stop - start).count());

			static auto idle_ratio		 = uint8_t {};
			static auto sleep_ratio		 = uint8_t {};
			static auto deep_sleep_ratio = uint8_t {};

			static auto stack_used_delta	= int32_t {};
			static auto stack_used_size		= uint32_t {};
			static auto stack_reserved_size = uint32_t {};
			static auto stack_used_ratio	= uint8_t {};

			static auto heap_used_delta	   = int32_t {};
			static auto heap_used_size	   = uint32_t {};
			static auto heap_reserved_size = uint32_t {};
			static auto heap_used_ratio	   = uint8_t {};

			while (true) {
				internal::instance.kick();
				++kick_count;

				stop  = rtos::Kernel::Clock::now();
				delta = static_cast<int>((stop - start).count());

				mbed_stats_cpu_get(&stats::cpu);

				idle_ratio	= static_cast<uint8_t>(((stats::cpu.idle_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				sleep_ratio = static_cast<uint8_t>(((stats::cpu.sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));
				deep_sleep_ratio =
					static_cast<uint8_t>(((stats::cpu.deep_sleep_time / 1000 * 100) / (stats::cpu.uptime / 1000)));

				mbed_stats_stack_get(&stats::stack);

				stack_used_delta	= static_cast<int32_t>(stats::stack.max_size - stack_used_size);
				stack_used_size		= stats::stack.max_size;
				stack_reserved_size = stats::stack.reserved_size;
				stack_used_ratio	= static_cast<uint8_t>((stack_used_size * 100) / stack_reserved_size);

				mbed_stats_heap_get(&stats::heap);

				heap_used_delta	   = static_cast<int32_t>(stats::heap.current_size - heap_used_size);
				heap_used_size	   = stats::heap.current_size;
				heap_reserved_size = stats::heap.reserved_size;
				heap_used_ratio	   = static_cast<uint8_t>((heap_used_size * 100) / heap_reserved_size);

				log_info(
					"dt: %i, kck: %u, idl: %u%%, slp: %u%%, dsl: %u%%, sur: %u%% (%+i)[%u/"
					"%u], hur: %u%% (%+i)[%u/%u]",
					delta, kick_count, idle_ratio, sleep_ratio, deep_sleep_ratio, stack_used_ratio, stack_used_delta,
					stack_used_size, stack_reserved_size, heap_used_ratio, heap_used_delta, heap_used_size,
					heap_reserved_size);

				start = rtos::Kernel::Clock::now();
				rtos::ThisThread::sleep_for(5s);
			}
		}

	}	// namespace internal

	void start()
	{
		internal::instance.start(internal::timeout.count());
		internal::thread.start(watchdog::internal::watchdog_kick);
	}

}	// namespace watchdog

}	// namespace

SDBlockDevice sd_blockdevice(SD_SPI_MOSI, SD_SPI_MISO, SD_SPI_SCK);
FATFileSystem fatfs("fs");

namespace display::internal {

auto event_loop = EventLoopKit {};
auto backlight	= CorePwm {SCREEN_BACKLIGHT_PWM};

auto corell		   = CoreLL {};
auto pixel		   = CGPixel {corell};
auto hal		   = CoreSTM32Hal {};
auto coresdram	   = CoreSDRAM {hal};
auto coredma2d	   = CoreDMA2D {hal};
auto coredsi	   = CoreDSI {hal};
auto coreltdc	   = CoreLTDC {hal};
auto coregraphics  = CoreGraphics {coredma2d};
auto corefont	   = CoreFont {pixel};
auto coreotm	   = CoreLCDDriverOTM8009A {coredsi, backlight};
auto corelcd	   = CoreLCD {coreotm};
auto _corejpegmode = CoreJPEGModeDMA {hal};
auto corejpeg	   = CoreJPEG {hal, _corejpegmode};

extern "C" auto corevideo =
	CoreVideo {hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg};

}	// namespace display::internal

auto videokit = VideoKit {display::internal::event_loop, display::internal::corevideo};

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
	watchdog::start();

	log_info("Hello, World!\n\n");

	rtos::ThisThread::sleep_for(2s);

	videokit.initializeScreen();

	initializeSD();

	HelloWorld hello;
	hello.start();

	display::internal::corevideo.clearScreen();
	rtos::ThisThread::sleep_for(1s);

	{
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

	display::internal::corelcd.enableDeepSleep();
	while (true) {
		rtos::ThisThread::sleep_for(10min);
	}
}
