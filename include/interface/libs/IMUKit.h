// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
namespace leka::interface {

struct EulerAngles {
	float pitch;
	float roll;
	float yaw;
};

class IMUKit
{
  public:
	using angles_ready_callback_t = std::function<void(const EulerAngles &)>;

	virtual ~IMUKit() = default;

	virtual void init()	 = 0;
	virtual void start() = 0;
	virtual void stop()	 = 0;

	virtual void setOrigin()													= 0;
	virtual void onEulerAnglesReady(angles_ready_callback_t const &callback)	= 0;
	[[nodiscard]] virtual auto getEulerAngles() const -> interface::EulerAngles = 0;
};

}	// namespace leka::interface
