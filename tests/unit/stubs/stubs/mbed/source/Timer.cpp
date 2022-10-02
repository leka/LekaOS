// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/Timer.h"

mbed::TimerBase::TimerBase(ticker_data_t const *data) : _ticker_data(data)
{
	// do nothing
}

mbed::Timer::Timer() : TimerBase(nullptr)
{
	// do nothing
}

mbed::TimerBase::~TimerBase() = default;

void mbed::TimerBase::start()
{
	// do nothing
}

void mbed::TimerBase::stop()
{
	// do nothing
}

auto mbed::TimerBase::elapsed_time() const -> std::chrono::microseconds
{
	return std::chrono::microseconds(0);
}
