// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "ComUtils.h"

#include "rtos/ThisThread.h"

using namespace leka;
using namespace std::chrono;

ComUtils::ComUtils(rtos::EventFlags &event_flags) : _event_flags(event_flags)
{
	_buffer.reserve(512);
	_event_queue.dispatch_forever();

	auto func = [this]() { this->read(); };
	_event_queue.call_every(200ms, func);
}

void ComUtils::read()
{
	_buffer.clear();

	while (_serial.readable()) {
		auto c = char {};

		_serial.read(&c, 1);

		if (c == '\n') {
			break;
		}

		_buffer.push_back(c);
		rtos::ThisThread::sleep_for(1ms);
	}

	auto data_available = !_buffer.empty();

	if (data_available) {
		_event_flags.set(flags::data_available);
	}
}

auto ComUtils::getPath() const -> std::filesystem::path
{
	if (_buffer.empty()) {
		return {};
	}

	return std::filesystem::path {_buffer};
}

void ComUtils::write(std::string_view message)
{
	static constexpr auto line_break = std::to_array({'\n'});

	_serial.write(message.data(), message.length());
	rtos::ThisThread::sleep_for(2ms);
	_serial.write(line_break.data(), std::size(line_break));
	rtos::ThisThread::sleep_for(2ms);
}
