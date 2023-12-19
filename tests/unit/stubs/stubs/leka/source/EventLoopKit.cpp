// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../EventLoopKit.h"

using namespace leka::stub;

void EventLoopKit::registerCallback(const callback_t &callback)
{
	_callback = callback;
}

void EventLoopKit::start(bool loop_once)
{
	if (_callback == nullptr) {
		return;
	}

	loops	  = 0;
	must_exit = false;
	loop();
}

void EventLoopKit::stop()
{
	must_exit = true;
}

void EventLoopKit::exit()
{
	must_exit = true;
}

void EventLoopKit::loop()
{
	auto keep_running = [&] {
		auto ret = (!must_exit && loops < spy_number_of_loops);
		++loops;
		return ret;
	};

	while (keep_running()) {
		_callback();
	}
}

void EventLoopKit::spy_setNumberOfLoops(int i)
{
	spy_number_of_loops = i;
}
