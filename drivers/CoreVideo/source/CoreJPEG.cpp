// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEG.hpp"

#include "external/st_jpeg_utils.h"
#include "internal/corevideo_config.h"

using namespace leka;

void CoreJPEG::initialize()
{
	JPEG_InitColorTables();
	_hal.HAL_RCC_JPEG_CLK_ENABLE();
	_hal.HAL_JPEG_Init(&_hjpeg);
}

auto CoreJPEG::getImageProperties() -> JPEGImageProperties
{
	auto image_properties = JPEGImageProperties {};
	_hal.HAL_JPEG_GetInfo(&_hjpeg, &image_properties);

	return image_properties;
}

auto CoreJPEG::getHandle() -> JPEG_HandleTypeDef
{
	return _hjpeg;
}

auto CoreJPEG::getHandlePointer() -> JPEG_HandleTypeDef *
{
	return &_hjpeg;
}

void CoreJPEG::decodeImage(interface::File *file)
{
	_mode.decode(&_hjpeg, file);
}
