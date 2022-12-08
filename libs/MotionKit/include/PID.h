// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <tuple>

#include "interface/drivers/Motor.h"

namespace leka {

class PID
{
  public:
	PID() = default;

	auto processPID([[maybe_unused]] float pitch, [[maybe_unused]] float roll, float yaw)
		-> std::tuple<float, Rotation>;

  private:
	// ? Kp, Ki, Kd were found empirically by increasing Kp until the rotation angle exceeds the target angle
	// ? Then increase Kd to fix this excess angle
	// ? Repeat this protocol until there is no Kd high enough to compensate Kp
	// ? Then take the last set of Kp, Kd value with no excess angle
	// ? Finally choose a low Ki that smooth out the movement
	struct Parameters {
		static constexpr auto Kp = float {0.3F};
		static constexpr auto Ki = float {0.0001F};
		static constexpr auto Kd = float {0.4F};
	};
	const float kStaticBound = 5.F;
	const float kDeltaT		 = 70.F;
	const float kTargetAngle = 180.F;

	float _error_position_total	  = 0.F;
	float _error_position_current = 0.F;
	float _error_position_last	  = 0.F;
	float _proportional			  = 0.F;
	float _integral				  = 0.F;
	float _derivative			  = 0.F;
};

}	// namespace leka
