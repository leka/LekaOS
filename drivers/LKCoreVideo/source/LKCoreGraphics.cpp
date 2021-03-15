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

void LKCoreGraphics::drawPixel(Pixel pixel, Color color)
{
	// TODO: Use a buffer and draw all in the same time. Need to determine buffer address

	_destination_address = lcd::frame_buffer_address + (4 * (pixel.y * lcd::dimension.width + pixel.x));
	_destinationColor	 = color.getARGB();

	rawMemoryWrite(_destination_address, _destinationColor);
}

void LKCoreGraphics::drawRectangle(FilledRectangle rectangle, Color color)
{
	_destination_address =
		lcd::frame_buffer_address + 4 * (lcd::dimension.width * rectangle.origin.y + rectangle.origin.x);

	_destinationColor = color.getARGB();

	_dma2d.transferDrawing(_destination_address, rectangle.width, rectangle.height, _destinationColor);
}

void LKCoreGraphics::rawMemoryWrite(uintptr_t destination, uint32_t data)
{
#if defined(__arm__)
	*reinterpret_cast<uintptr_t *>(destination) = data;
#endif
}

uintptr_t LKCoreGraphics::getDestinationAddress()
{
	return _destination_address;
}

uint32_t LKCoreGraphics::getDestinationColor()
{
	return _destinationColor;
}

}	// namespace leka
