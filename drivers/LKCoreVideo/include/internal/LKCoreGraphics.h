// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_GRAPHICS_H_
#define _LEKA_OS_LIB_GRAPHICS_H_

#include "CGColor.h"
#include "LKCoreDMA2DBase.h"
#include "LKCoreGraphicsBase.h"
#include "corevideo_config.h"

namespace leka {

class LKCoreGraphics : public LKCoreGraphicsBase
{
  public:
	LKCoreGraphics(LKCoreDMA2DBase &dma2d);

	void clearScreen(Color color = CGColor::white) final;

	void drawPixel(Pixel pixel, Color color) final;
	void drawRectangle(FilledRectangle rectangle, Color color) final;

	uintptr_t getDestinationAddress() final;
	uint32_t getDestinationColor() final;

  private:
	void rawMemoryWrite(uintptr_t destination, uint32_t data);

  private:
	LKCoreDMA2DBase &_dma2d;

	uintptr_t _destination_address;
	uint32_t _destinationColor;
};

}	// namespace leka

#endif
