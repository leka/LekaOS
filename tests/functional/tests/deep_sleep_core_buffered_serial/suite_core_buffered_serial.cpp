// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "CoreBufferedSerial.h"
#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace leka;
using namespace boost::ut;
using namespace std::chrono;
using namespace boost::ut::bdd;

suite suite_core_buffered_serial = [] {
	scenario("base system check") = [] {
		given("serial is not instantiated") = [] {
			then("I expect deep sleep TO BE possible") = [] {
				auto status = tests::utils::sleep::system_deep_sleep_check();

				expect(status.test_check_ok);
			};
		};
	};

	scenario("buffered serial initialization") = [] {
		given("buffered serial is in default configuration") = [] {
			auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);

			expect(neq(&serial, nullptr));

			rtos::ThisThread::sleep_for(500ms);

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = tests::utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("enabled/disable deepsleep") = [] {
		given("serial is in default configuration") = [] {
			auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);

			expect(neq(&serial, nullptr));

			rtos::ThisThread::sleep_for(500ms);

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = tests::utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};

			when("I enable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				serial.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = tests::utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I disable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				serial.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = tests::utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};

			when("I enable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				serial.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = tests::utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I disable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				serial.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = tests::utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};

			when("I enable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				serial.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = tests::utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I disable deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				serial.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = tests::utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};
		};
	};
};
