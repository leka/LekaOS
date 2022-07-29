// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka::interface {

class Game
{
  public:
	virtual ~Game() = default;

	virtual void start() = 0;
	virtual void run()	 = 0;
	virtual void stop()	 = 0;

	[[nodiscard]] virtual auto isRunning() const -> bool = 0;
};

}	// namespace leka::interface
