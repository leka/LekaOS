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

suite suite_core_lcd = [] {
	scenario("base system check") = [] {
		given("core lcd is not instantiated") = [] {
			then("I expect deep sleep TO BE possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(status.can_deep_sleep);
				expect(status.test_check_ok);
			};
		};
	};

	auto hal	  = CoreSTM32Hal {};
	auto coredsi  = CoreDSI {hal};
	auto coreltdc = CoreLTDC {hal};
	auto coreotm  = CoreLCDDriverOTM8009A {coredsi, PinName::SCREEN_BACKLIGHT_PWM};
	auto corelcd  = CoreLCD {coreotm};

	auto init_core_lcd = [&] {
		hal.HAL_RCC_LTDC_CLK_ENABLE();
		hal.HAL_RCC_LTDC_FORCE_RESET();
		hal.HAL_RCC_LTDC_RELEASE_RESET();

		coredsi.initialize();
		coreltdc.initialize();

		coredsi.start();

		corelcd.initialize();
	};

	scenario("initialization") = [&] {
		given("core lcd is in default configuration") = [&] {
			expect(neq(&corelcd, nullptr));

			when("I do nothing") = [] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I initialize") = [&] {
				init_core_lcd();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("init, on, off, on, off") = [&] {
		given("core video is initialized") = [&] {
			when("I turn on") = [&] {
				corelcd.turnOn();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I turn off") = [&] {
				corelcd.turnOff();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("I turn on again") = [&] {
				corelcd.turnOn();
				rtos::ThisThread::sleep_for(250ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I turn off agin") = [&] {
				corelcd.turnOff();
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
