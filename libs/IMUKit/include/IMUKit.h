// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

#include "interface/Accelerometer.h"
#include "interface/Gyroscope.h"
#include "interface/libs/EventLoop.h"
#include "internal/Mahony.h"

namespace leka {

class IMUKit
{
  public:
	IMUKit(interface::EventLoop &event_loop, interface::Accelerometer &accel, interface::Gyroscope &gyro)
		: _event_loop(event_loop),
		  _accel(accel),
		  _gyro(gyro) {
			  // nothing to do
		  };

	void init();
	void start();
	void run();
	void stop();

	auto getAngles() -> std::array<float, 3>;

	void reset();

	void computeAngles();

  private:
	interface::EventLoop &_event_loop;
	interface::Accelerometer &_accel;
	interface::Gyroscope &_gyro;

	ahrs::Mahony _mahony {};
	const std::chrono::milliseconds _delay = std::chrono::milliseconds {70};
	bool _is_running {false};
};

}	// namespace leka
