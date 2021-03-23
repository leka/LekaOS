// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreGraphics.h"

namespace leka {

LKCoreGraphics::LKCoreGraphics(LKCoreDMA2DBase &dma2d) : _dma2d(dma2d) {}

void LKCoreGraphics::clearScreen(Color color)
{
	FilledRectangle rect;
	rect.width	= lcd::dimension.width;
	rect.height = lcd::dimension.height;

	drawRectangle(rect, color);
}

void LKCoreGraphics::drawRectangle(FilledRectangle rectangle, Color color)
{
	uintptr_t destination_address =
		lcd::frame_buffer_address + 4 * (lcd::dimension.width * rectangle.origin.y + rectangle.origin.x);

	uint32_t destinationColor = color.getARGB();

	_dma2d.transferDrawing(destination_address, rectangle.width, rectangle.height, destinationColor);
}

}	// namespace leka
