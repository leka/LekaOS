// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_H_
#define _LEKA_OS_LIB_GRAPHICS_H_

#include "CGColor.h"
#include "interface/LKCoreDMA2D.h"
#include "interface/LKCoreGraphics.h"
#include "LKCoreLL.h"
#include "corevideo_config.h"

namespace leka {

class LKCoreGraphics : public interface::LKCoreGraphics
{
  public:
	explicit LKCoreGraphics(interface::LKCoreDMA2D &dma2d);

	void clearScreen(CGColor color = CGColor::white) final;

	void drawRectangle(FilledRectangle rectangle, CGColor color) final;

  private:
	interface::LKCoreDMA2D &_dma2d;
};

}	// namespace leka

#endif
