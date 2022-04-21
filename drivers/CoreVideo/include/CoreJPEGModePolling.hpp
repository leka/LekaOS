// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>

#include "external/st_jpeg_utils.h"
#include "interface/JPEGMode.hpp"
#include "internal/corevideo_config.h"

namespace leka {

class CoreJPEGModePolling : public interface::JPEGMode
{
  public:
	explicit CoreJPEGModePolling(interface::STM32Hal &hal) : _hal(hal) {}

	void onMspInitCallback(JPEG_HandleTypeDef *hjpeg) final;
	void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) final;
	void onErrorCallback(JPEG_HandleTypeDef *hjpeg) final;
	void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t datasize) final;
	void onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize) final;
	void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) final;

	auto decode(JPEG_HandleTypeDef *hjpeg, interface::File &file) -> size_t final;

  private:
	struct JPEGDataBuffer {
		uint8_t *data;
		uint32_t size;
	};

	interface::STM32Hal &_hal;
	interface::File *_file {};

	size_t _image_size			= 0;
	uint32_t _mcu_number		= 0;
	uint32_t _mcu_block_index	= 0;
	uint32_t _input_file_offset = 0;

	std::array<uint8_t, leka::jpeg::output_chunk_size> _mcu_data_output_buffer {0};
	std::array<uint8_t, leka::jpeg::input_chunk_size> _jpeg_data_output_buffer {0};

	JPEG_YCbCrToRGB_Convert_Function pConvert_Function {};

	// TODO(@yann): do we really need this struct?
	JPEGDataBuffer _jpeg_input_buffer = {_jpeg_data_output_buffer.data(), 0};
};

}	// namespace leka
