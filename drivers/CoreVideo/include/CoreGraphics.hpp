// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_H_
#define _LEKA_OS_LIB_GRAPHICS_H_

#include "CGColor.hpp"
#include "LKCoreLL.h"
#include "interface/DMA2D.hpp"
#include "interface/CoreGraphics.hpp"
#include "internal/corevideo_config.h"

namespace leka {

class CoreGraphics : public interface::CoreGraphics
{
  public:
	explicit CoreGraphics(interface::CoreDMA2D &dma2d);

	void clearScreen(CGColor color = CGColor::white) final;

	void drawRectangle(FilledRectangle rectangle, CGColor color) final;

  private:
	interface::CoreDMA2D &_dma2d;
};

}	// namespace leka

#endif
