// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBufferedSerial.h"
#include "CoreRFIDReaderCR95HF.h"
#include "RFIDKit.h"
#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace leka;
using namespace boost::ut;
using namespace std::chrono;
using namespace boost::ut::bdd;

auto rfidserial = CoreBufferedSerial(RFID_UART_TX, RFID_UART_RX, 57600);
auto rfidreader = CoreRFIDReaderCR95HF(rfidserial);
auto rfidkit	= RFIDKit(rfidreader);

suite suite_rfid_kit = [] {
	scenario("rfid initialization") = [] {
		given("rfid is in default configuration") = [] {
			rfidkit.init();
			rtos::ThisThread::sleep_for(5ms);

			expect(neq(&rfidkit, nullptr));

			when("I do nothing") = [&] {
				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("enabled/disable deepsleep") = [] {
		given("rfid is in default configuration") = [] {
			when("I enable rfid deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				rfidkit.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I disable rfid deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				rfidkit.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};

			when("I enable rfid deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				rfidkit.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I disable rfid deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				rfidkit.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};
		};
	};

	scenario("enabled/disable deepsleep then magic card detected") = [] {
		given("rfid is in default configuration") = [] {
			auto cardDetected = false;
			rfidkit.onTagActivated([&cardDetected](const MagicCard &card) { cardDetected = true; });

			when("I wait") = [&] {
				cardDetected = false;
				rtos::ThisThread::sleep_for(1s);

				then("I expect card TO BE detected") = [&cardDetected] { expect(cardDetected); };
			};

			when("I enable rfid deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				rfidkit.enableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(status.test_check_ok);
				};
			};

			when("I wait") = [&] {
				cardDetected = false;
				rtos::ThisThread::sleep_for(1s);

				then("I expect card TO NOT BE detected") = [&cardDetected] { expect(not cardDetected); };
			};

			when("I disable rfid deep sleep") = [&] {
				rtos::ThisThread::sleep_for(500ms);
				rfidkit.disableDeepSleep();
				rtos::ThisThread::sleep_for(500ms);

				then("I expect deep sleep TO NOT BE possible") = [] {
					auto status = utils::sleep::system_deep_sleep_check();

					expect(not status.test_check_ok);
				};
			};

			when("I wait") = [&] {
				cardDetected = false;
				rtos::ThisThread::sleep_for(1s);

				then("I expect card TO BE detected") = [&cardDetected] { expect(cardDetected); };
			};
		};
	};
};
