// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/STM32Hal.h"
#include "interface/platform/File.h"

namespace leka::interface {

struct JPEGMode {
	virtual ~JPEGMode() = default;

	virtual void onMspInitCallback(JPEG_HandleTypeDef *hjpeg)											 = 0;
	virtual void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)					 = 0;
	virtual void onErrorCallback(JPEG_HandleTypeDef *hjpeg)												 = 0;
	virtual void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t datasize) = 0;
	virtual void onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize)			 = 0;
	virtual void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)									 = 0;

	virtual auto decode(JPEG_HandleTypeDef *hjpeg, interface::File &file) -> size_t = 0;
};

}	// namespace leka::interface
