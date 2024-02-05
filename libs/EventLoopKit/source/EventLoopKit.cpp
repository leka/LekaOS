// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "EventLoopKit.h"

using namespace leka;

EventLoopKit::EventLoopKit()
{
	_thread.start({this, &EventLoopKit::loop});
}

void EventLoopKit::registerCallback(const callback_t &callback)
{
	_callback = callback;
}

void EventLoopKit::start(bool loop_once)
{
	if (_callback == nullptr) {
		return;
	}
	_loop_once = loop_once;

	_flags.set(flag::START);
}

void EventLoopKit::stop()
{
	_flags.clear(flag::START);
}

void EventLoopKit::exit()
{
	must_exit = true;
}

// LCOV_EXCL_START
void EventLoopKit::loop()
{
	auto keep_running = [&] {
		auto ret = (!must_exit);
		return ret;
	};

	auto should_clear = [&] { return _loop_once; };

	while (keep_running()) {
		_flags.wait_any(flag::START, should_clear());

		_callback();
	}
}
// LCOV_EXCL_STOP
