// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

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
#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace leka;
using namespace boost::ut;
using namespace std::chrono;
using namespace boost::ut::bdd;

auto backlight = CorePwm {SCREEN_BACKLIGHT_PWM};

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

suite suite_core_lcd = [] {
	scenario("lcd initialization") = [] {
		given("lcd is in default configuration") = [] {
			corevideo.initialize();
			rtos::ThisThread::sleep_for(5ms);

			expect(neq(&corelcd, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("enabled/disable deepsleep") = [] {
		given("lcd is in default configuration") = [] {
			when("I enable lcd deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				corelcd.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("I disable lcd deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				corelcd.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I enable lcd deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				corelcd.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("I disable lcd deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				corelcd.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};
};
