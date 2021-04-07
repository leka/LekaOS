// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_H_
#define _LEKA_OS_LIB_GRAPHICS_H_

#include "CGColor.h"
#include "LKCoreDMA2DBase.h"
#include "LKCoreGraphicsBase.h"
#include "LKCoreLL.h"
#include "corevideo_config.h"

namespace leka {

class LKCoreGraphics : public LKCoreGraphicsBase
{
  public:
	explicit LKCoreGraphics(LKCoreDMA2DBase &dma2d);

	void clearScreen(Color color = CGColor::white) final;

	void drawRectangle(FilledRectangle rectangle, Color color) final;

  private:
	LKCoreDMA2DBase &_dma2d;
};

}	// namespace leka

#endif
