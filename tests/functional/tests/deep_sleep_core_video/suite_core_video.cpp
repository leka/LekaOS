// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreDMA2D.hpp"
#include "CoreDSI.hpp"
#include "CoreFont.hpp"
#include "CoreGraphics.hpp"
#include "CoreJPEG.hpp"
#include "CoreJPEGModeDMA.hpp"
#include "CoreLCD.hpp"
#include "CoreLTDC.hpp"
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
auto corevideo = CoreVideo {hal, coresdram, coredma2d, coredsi, coreltdc, corelcd, coregraphics, corefont, corejpeg};

HAL_VIDEO_DECLARE_IRQ_HANDLERS(corevideo);

suite suite_core_video = [] {
	scenario("core video initialization") = [] {
		given("core video is in default configuration") = [] {
			rtos::ThisThread::sleep_for(250ms);

			expect(neq(&corevideo, nullptr));

			when("I do nothing") = [] {
				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};
		};
	};

	scenario("init, on, off, on, off") = [] {
		given("core video is in default configuration") = [] {
			rtos::ThisThread::sleep_for(250ms);

			expect(neq(&corevideo, nullptr));

			when("I initialize") = [] {
				corevideo.initialize();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I turn on") = [] {
				corevideo.turnOn();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I turn off") = [] {
				corevideo.turnOff();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("I turn on") = [] {
				corevideo.turnOn();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I turn off") = [] {
				corevideo.turnOff();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};
		};
	};

	scenario("fill screen when off") = [] {
		given("core video is off") = [] {
			corevideo.initialize();
			corevideo.turnOff();
			rtos::ThisThread::sleep_for(250ms);

			then("I expect deep sleep TO BE possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(status.can_deep_sleep);
				expect(status.test_check_ok);
			};

			when("I clear screen in red") = [] {
				corevideo.clearScreen(CGColor::pure_red);
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("I turn on and set brightness to 100%") = [] {
				corevideo.turnOn();
				corevideo.setBrightness(1.0);
				rtos::ThisThread::sleep_for(1000ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I turn off") = [] {
				corevideo.turnOff();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};
		};
	};

	scenario("fill screen when on") = [] {
		given("core video is on with brightness at 100%") = [] {
			corevideo.initialize();
			corevideo.turnOn();
			corevideo.setBrightness(1.0);
			rtos::ThisThread::sleep_for(250ms);

			then("I expect deep sleep TO NOT BE possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(not status.can_deep_sleep);
				expect(not status.test_check_ok);
			};

			when("I clear screen in blue") = [] {
				corevideo.clearScreen(CGColor::pure_blue);
				rtos::ThisThread::sleep_for(1000ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I turn off") = [] {
				corevideo.turnOff();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};
		};
	};
};
