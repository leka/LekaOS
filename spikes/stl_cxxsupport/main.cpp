// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <lstd_array>
#include <lstd_span>

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "HelloWorld.h"
#include "LogKit.h"

using namespace leka;
using namespace std::chrono;

void foo(lstd::span<int> span)
{
	log_info("from foo span size: %i", span.size());
	for (const auto &v: span) {
		log_info("v: %i", v);
	}
}

auto main() -> int
{
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);
	leka::logger::set_sink_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("Hello, World!\n");

	rtos::ThisThread::sleep_for(2s);

	auto hello = HelloWorld();
	hello.start();

	//
	// MARK: - lstd::to_array
	//

	log_info("Create array with lstd::to_array");

	auto arr = lstd::to_array({1, 2, 3, 4});

	for (const auto &v: arr) {
		log_info("v: %i", v);
	}

	//
	// MARK: - lstd::span
	//

	log_info("Pass std::array to lstd::span");

	foo(arr);

	log_info("Create span from array, auto deduce size");

	auto span0 = lstd::span {arr};

	for (const auto &v: span0) {
		log_info("v: %i", v);
	}

	foo(span0);

	log_info("Create span from begin pointer and size");

	auto span1 = lstd::span {arr.begin(), 3};

	for (const auto &v: span1) {
		log_info("v: %i", v);
	}

	foo(span1);

	log_info("Create span from specific i-th element and size - i");

	auto span2 = lstd::span {arr.begin() + 2, arr.size() - 2};

	for (const auto &v: span2) {
		log_info("v: %i", v);
	}

	foo(span2);

	log_info("Test span features");

	auto value = span0[3];
	log_info("span0[3] = %i", value);

	auto front = span0.front();
	log_info("span0.front() = %i", front);

	auto back = span0.back();
	log_info("span0.back() = %i", back);

	return 1;
}
