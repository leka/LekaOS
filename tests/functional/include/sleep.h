// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "rtos/ThisThread.h"

#include "boost/ut.hpp"

using namespace std::chrono;
using namespace boost::ut;

void busy_sleep(uint64_t milliseconds)
{
	for (uint64_t now = 0; now < milliseconds; ++now) {
		rtos::ThisThread::sleep_for(1ms);
		log << ".";
	}
	log << "\n";
}
