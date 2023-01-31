// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CGGraphics.hpp"
#include "CoreLL.h"
#include "interface/DMA2D.hpp"
#include "interface/Graphics.hpp"
#include "internal/corevideo_config.h"

namespace leka {

class CoreGraphics : public interface::Graphics
{
  public:
	explicit CoreGraphics(interface::DMA2DBase &dma2d);

	void clearScreen(CGColor color = CGColor::white) final;

	void drawRectangle(CGRectangle rectangle, CGColor color) final;

  private:
	interface::DMA2DBase &_dma2d;
};

}	// namespace leka
