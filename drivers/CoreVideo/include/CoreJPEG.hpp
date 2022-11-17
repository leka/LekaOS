// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>

#include "JPEGImageProperties.hpp"
#include "interface/JPEG.hpp"
#include "interface/JPEGMode.hpp"
#include "interface/drivers/STM32Hal.h"
#include "interface/platform/File.h"

namespace leka {

class CoreJPEG : public interface::JPEGBase
{
  public:
	explicit CoreJPEG(interface::STM32Hal &hal, interface::JPEGMode &mode) : _hal(hal), _mode(mode)
	{
		_hjpeg.Instance = JPEG;
	}

	void initialize() final;

	auto getHandle() -> JPEG_HandleTypeDef & final;

	auto decodeImage(interface::File &file) -> std::size_t final;
	auto findSOIMarker(interface::File &file, std::size_t start_index) -> std::size_t final;

	auto getImageProperties() -> JPEGImageProperties final;

  private:
	void registerInitializationCallbacks();
	void registerProcessCallbacks();

	JPEG_HandleTypeDef _hjpeg {};
	interface::STM32Hal &_hal;
	interface::JPEGMode &_mode;
};

}	// namespace leka
