// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/Accelerometer.h"
#include "interface/LSM6DSOX.h"

namespace leka {

using mg_t = float;

class CoreAccelerometer : public interface::Accelerometer
{
  public:
	explicit CoreAccelerometer(interface::LSM6DSOX &lsm6dsox) : _lsm6dsox(lsm6dsox) {};

	auto getXYZ() -> std::tuple<mg_t, mg_t, mg_t> final;

  private:
	interface::LSM6DSOX &_lsm6dsox;
};

}	// namespace leka
