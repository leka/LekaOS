// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/Gyroscope.h"
#include "interface/LSM6DSOX.h"

namespace leka {

using dps_t = float;

class CoreGyroscope : public interface::Gyroscope
{
  public:
	explicit CoreGyroscope(interface::LSM6DSOX &lsm6dsox) : _lsm6dsox(lsm6dsox) {};

	auto getXYZ() -> std::tuple<dps_t, dps_t, dps_t> final;

  private:
	interface::LSM6DSOX &_lsm6dsox;
};

}	// namespace leka
