// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka {

enum class Rotation
{
	clockwise		 = 0,
	counterClockwise = 1,

	right = clockwise,
	left  = counterClockwise
};

using rotation_t = Rotation;

namespace interface {

	class Motor

	{
	  public:
		virtual ~Motor() = default;

		virtual void spin(rotation_t rotation, float speed) = 0;
		virtual void stop()									= 0;
	};

}	// namespace interface
}	// namespace leka
