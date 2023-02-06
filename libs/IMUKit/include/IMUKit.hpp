// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <chrono>

#include "interface/LSM6DSOX.hpp"

namespace leka {

class IMUKit
{
  public:
	explicit IMUKit(interface::LSM6DSOX &lsm6dsox) : _lsm6dsox(lsm6dsox) {}

	void init();
	void start();
	void stop();

	void setOrigin();
	auto getAngles() -> std::array<float, 3>;

  private:
	interface::LSM6DSOX &_lsm6dsox;
};

}	// namespace leka
