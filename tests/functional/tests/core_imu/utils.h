// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <tuple>
#include <vector>

#include "rtos/ThisThread.h"

#include "boost/ut.hpp"
#include "interface/LSM6DSOX.h"

inline auto values_did_change_over_time(leka::interface::LSM6DSOX &lsm6dsox)
{
	using namespace std::chrono;
	using namespace boost::ut;

	auto sensor_data = leka::interface::LSM6DSOX::SensorData();

	auto i_batch = std::vector<float> {};
	auto f_batch = std::vector<float> {};

	auto sensor_callback = [&](const leka::interface::LSM6DSOX::SensorData &data) { sensor_data = data; };
	lsm6dsox.registerOnGyDataReadyCallback(sensor_callback);

	for (auto i = 0; i < 10; ++i) {
		auto [xlx, xly, xlz] = sensor_data.xl;

		i_batch.push_back(xlx);
		i_batch.push_back(xly);
		i_batch.push_back(xlz);

		auto [gyx, gyy, gyz] = sensor_data.gy;

		i_batch.push_back(gyx);
		i_batch.push_back(gyy);
		i_batch.push_back(gyz);

		rtos::ThisThread::sleep_for(25ms);
	}

	rtos::ThisThread::sleep_for(100ms);

	for (auto i = 0; i < 10; ++i) {
		auto [xlx, xly, xlz] = sensor_data.xl;

		f_batch.push_back(xlx);
		f_batch.push_back(xly);
		f_batch.push_back(xlz);

		auto [gyx, gyy, gyz] = sensor_data.gy;

		f_batch.push_back(gyx);
		f_batch.push_back(gyy);
		f_batch.push_back(gyz);

		rtos::ThisThread::sleep_for(25ms);
	}

	return neq(i_batch, f_batch);
}
