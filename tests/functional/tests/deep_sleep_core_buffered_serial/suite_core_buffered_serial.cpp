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
	scenario("default configuration") = [] {
		given("serial is in default configuration") = [] {
			auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);

			expect(neq(&serial, nullptr));

			when("I do nothing") = [] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("disable input") = [] {
		given("serial is in default configuration") = [] {
			auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);

			expect(neq(&serial, nullptr));

			when("I disable input") = [&] {
				serial.disable_input();
				rtos::ThisThread::sleep_for(5ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};
		};
	};

	scenario("default, disable, enable, disable input") = [] {
		given("serial is in default configuration") = [] {
			auto serial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);

			expect(neq(&serial, nullptr));

			when("I do nothing") = [] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I disable input") = [&] {
				serial.disable_input();
				rtos::ThisThread::sleep_for(5ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};

			when("I enable input") = [&] {
				serial.enable_input();

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.can_deep_sleep);
					expect(not status.test_check_ok);
				};
			};

			when("I disable input") = [&] {
				serial.disable_input();
				rtos::ThisThread::sleep_for(5ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.can_deep_sleep);
					expect(status.test_check_ok);
				};
			};
		};
	};
};
