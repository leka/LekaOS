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

void CoreJPEG::registerCallbacks()
{
	static auto &self = (*this);

	_hal.HAL_JPEG_RegisterInfoReadyCallback(&_hjpeg, [](JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) {
		self._mode.onInfoReadyCallback(hjpeg, info);
	});

	_hal.HAL_JPEG_RegisterGetDataCallback(&_hjpeg, [](JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize) {
		self._mode.onDataAvailableCallback(hjpeg, decoded_datasize);
	});

	_hal.HAL_JPEG_RegisterDataReadyCallback(&_hjpeg,
											[](JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t datasize) {
												self._mode.onDataReadyCallback(hjpeg, output_data, datasize);
											});

	_hal.HAL_JPEG_RegisterCallback(&_hjpeg, HAL_JPEG_DECODE_CPLT_CB_ID,
								   [](JPEG_HandleTypeDef *hjpeg) { self._mode.onDecodeCompleteCallback(hjpeg); });

	_hal.HAL_JPEG_RegisterCallback(&_hjpeg, HAL_JPEG_ERROR_CB_ID,
								   [](JPEG_HandleTypeDef *hjpeg) { self._mode.onErrorCallback(hjpeg); });

	_hal.HAL_JPEG_RegisterCallback(&_hjpeg, HAL_JPEG_MSPINIT_CB_ID,
								   [](JPEG_HandleTypeDef *hjpeg) { self._mode.onMspInitCallback(hjpeg); });
}

void CoreJPEG::decodeImage(interface::File *file)
{
	_mode.decode(&_hjpeg, file);
}
