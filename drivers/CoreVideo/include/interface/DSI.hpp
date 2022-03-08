// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "stm32f7xx_hal.h"

namespace leka::interface {

class DSIBase
{
  public:
	virtual ~DSIBase() = default;

	virtual void initialize() = 0;
	virtual void start()	  = 0;
	virtual void reset()	  = 0;
	virtual void refresh()	  = 0;

	struct SyncProps {
		int hsync;
		int hbp;
		int activew;
		int hfp;
		int vsync;
		int vbp;
		int activeh;
		int vfp;
	};
	virtual auto getSyncProps() -> SyncProps = 0;

	virtual void enableLPCmd()	= 0;
	virtual void disableLPCmd() = 0;

	virtual void enableTearingEffectReporting() = 0;

	virtual auto getHandle() -> DSI_HandleTypeDef & = 0;

	virtual void write(const uint8_t *data, uint32_t size) = 0;
};

}	// namespace leka::interface
