// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_H_
#define _LEKA_OS_LIB_GRAPHICS_H_

#include "CGColor.h"
#include "LKCoreGraphicsBase.h"
#include "LKCoreLL.h"
#include "corevideo_config.h"
#include "interface/CoreDMA2D.h"

namespace leka {

class LKCoreGraphics : public LKCoreGraphicsBase
{
  public:
	explicit LKCoreGraphics(interface::CoreDMA2D &dma2d);

	void clearScreen(CGColor color = CGColor::white) final;

	void drawRectangle(FilledRectangle rectangle, CGColor color) final;

  private:
	interface::CoreDMA2D &_dma2d;
};

}	// namespace leka

#endif
