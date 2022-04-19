// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "JPEGImageProperties.hpp"
#include "interface/drivers/STM32Hal.h"
#include "interface/platform/File.h"

namespace leka::interface {

class JPEGBase
{
  public:
	virtual ~JPEGBase() = default;

	virtual void initialize() = 0;

	virtual auto getHandle() -> JPEG_HandleTypeDef			= 0;
	virtual auto getHandlePointer() -> JPEG_HandleTypeDef * = 0;

	virtual void decodeImage(interface::File &) = 0;

	virtual auto getImageProperties() -> JPEGImageProperties = 0;
};

}	// namespace leka::interface
