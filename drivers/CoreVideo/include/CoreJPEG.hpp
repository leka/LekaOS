// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "CoreJPEGMode.h"
#include "external/st_jpeg_utils.h"
#include "interface/JPEG.hpp"
#include "interface/drivers/STM32Hal.h"
#include "interface/platform/File.h"

namespace leka {

class CoreJPEG : public interface::JPEGBase
{
  public:
	CoreJPEG(interface::STM32Hal &hal, std::unique_ptr<CoreJPEGMode> mode);

	void initialize() final;

	auto getHandle() -> JPEG_HandleTypeDef & final;
	auto getConfig() -> JPEG_ConfTypeDef final;

	void registerCallbacks() final;

	auto decodeImage(interface::File &file) -> uint32_t final;

	static auto getWidthOffset(JPEG_ConfTypeDef &config) -> uint32_t;
	static auto findFrameOffset(interface::File &file, uint32_t offset) -> uint32_t;

  private:
	// struct JPEGDataBuffer {
	// 	uint8_t *data;
	// 	uint32_t size;
	// };

	// std::array<uint8_t, leka::jpeg::mcu::output_data_buffer_size> _mcu_data_output_buffer {0};
	// std::array<uint8_t, leka::jpeg::input_data_buffer_size> _jpeg_data_output_buffer {0};

	// // TODO(@yann): do we really need this struct?
	// JPEGDataBuffer _jpeg_input_buffer = {_jpeg_data_output_buffer.data(), 0};

	// JPEG_HandleTypeDef _hjpeg {};
	// JPEG_ConfTypeDef _config {};
	interface::STM32Hal &_hal;
	// interface::DMA2DBase &_dma2d;
	// interface::File *_file;

	std::unique_ptr<CoreJPEGMode> _mode;

	JPEG_HandleTypeDef _hjpeg;
};

}	// namespace leka
