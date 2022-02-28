// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "storage/filesystem/fat/ChaN/ff.h"

#include "interface/drivers/STM32Hal.h"

namespace leka::interface {

class JPEGBase
{
  public:
	~JPEGBase() = default;

	virtual void initialize() = 0;

	virtual auto getConfig() -> JPEG_ConfTypeDef			= 0;
	virtual auto getHandle() -> JPEG_HandleTypeDef			= 0;
	virtual auto getHandlePointer() -> JPEG_HandleTypeDef * = 0;

	virtual auto getWidthOffset() -> uint32_t = 0;

	virtual void displayImage(FIL *file) = 0;
	// TODO(@yann): Update Return type with something else than HAL status
	virtual auto decodeImageWithPolling() -> HAL_StatusTypeDef = 0;

	virtual void onErrorCallback(JPEG_HandleTypeDef *hjpeg)								= 0;
	virtual void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) = 0;

	virtual void onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)					   = 0;
	virtual void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) = 0;

	virtual void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) = 0;
};

}	// namespace leka::interface
