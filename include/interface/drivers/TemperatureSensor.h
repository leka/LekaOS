// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka::interface {

class TemperatureSensor
{
  public:
	virtual ~TemperatureSensor() = default;

	virtual auto getTemperatureCelsius() -> float = 0;
};

class HumiditySensor
{
  public:
	virtual ~HumiditySensor() = default;

	virtual auto getRelativeHumidity() -> float = 0;
};

}	// namespace leka::interface
