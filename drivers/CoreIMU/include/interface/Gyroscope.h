// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <tuple>
namespace leka::interface {

class Gyroscope
{
  public:
	virtual ~Gyroscope() = default;

	virtual auto getXYZ() -> std::tuple<float, float, float> = 0;
};
}	// namespace leka::interface
