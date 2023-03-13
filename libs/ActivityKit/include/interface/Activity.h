// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

namespace leka::interface {

class Activity
{
  public:
	virtual ~Activity() = default;

	virtual void start(const std::function<void()> &before_process_callback) = 0;
	virtual void stop()														 = 0;
};

}	// namespace leka::interface
