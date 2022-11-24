// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <vector>

#include "rtos/ThisThread.h"

#include "boost/ut.hpp"

template <typename T>
concept CanGetDataXYZ = requires { &T::getXYZ; };

template <CanGetDataXYZ T>
auto values_did_change_over_time(T &sensor)
{
	using namespace std::chrono;
	using namespace boost::ut;

	auto i_batch = std::vector<float> {};
	auto f_batch = std::vector<float> {};

	for (auto i = 0; i < 10; ++i) {
		auto [x, y, z] = sensor.getXYZ();

		i_batch.push_back(x);
		i_batch.push_back(y);
		i_batch.push_back(z);

		rtos::ThisThread::sleep_for(25ms);
	}

	rtos::ThisThread::sleep_for(100ms);

	for (auto i = 0; i < 10; ++i) {
		auto [x, y, z] = sensor.getXYZ();

		f_batch.push_back(x);
		f_batch.push_back(y);
		f_batch.push_back(z);

		rtos::ThisThread::sleep_for(25ms);
	}

	return neq(i_batch, f_batch);
}
