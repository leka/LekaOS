// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>

#include "external/st_jpeg_utils.h"
#include "interface/JPEGMode.hpp"
#include "interface/drivers/STM32Hal.h"
#include "internal/corevideo_config.h"
#include "stm32f7xx_hal_jpeg.h"

namespace leka {

struct CoreJPEGMode : interface::JPEGMode {
	void onMspInitCallback(JPEG_HandleTypeDef *hjpeg) override;

	void onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) override;

	void onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) override;

	void onErrorCallback(JPEG_HandleTypeDef *hjpeg) override;

  protected:
	CoreJPEGMode() = default;
	virtual void reset();

	// color conversion function pointer, set by onInfoReadyCallback
	JPEG_YCbCrToRGB_Convert_Function convertMCUBlocks;

	uint32_t _image_size	  = 0;
	uint32_t _mcu_number	  = 0;
	uint32_t _mcu_block_index = 0;
	bool _hw_decode_ended	  = false;
};

struct CoreJPEGModeDMA : CoreJPEGMode {
	CoreJPEGModeDMA() = default;

	auto decodeImage(JPEG_HandleTypeDef *hjpeg, interface::File &file) -> uint32_t final;

	void onMspInitCallback(JPEG_HandleTypeDef *hjpeg) final;
	void onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize) final;
	void onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t datasize) final;

  private:
	static std::array<uint8_t, jpeg::input_chunk_size * jpeg::input_buffers_nb> BIG_CHUNGUS_OF_MEMORY_IN;
	static std::array<uint8_t, jpeg::output_chunk_size * jpeg::output_buffers_nb> BIG_CHUNGUS_OF_MEMORY_OUT;

	void reset() final;
	void decoderInputHandler(JPEG_HandleTypeDef *hjpeg, interface::File &file);
	auto decoderOutputHandler(JPEG_HandleTypeDef *hjpeg) -> bool;

	DMA_HandleTypeDef _hdma_in;
	DMA_HandleTypeDef _hdma_out;

	struct Buffer {
		enum State
		{
			Empty,
			Full
		};
		State state;
		uint8_t *data;
		uint32_t datasize;
	};

	std::array<Buffer, jpeg::input_buffers_nb> _input_buffers;
	std::array<Buffer, jpeg::output_buffers_nb> _output_buffers;

	uint32_t _input_buffers_read_index	 = 0;
	uint32_t _input_buffers_write_index	 = 0;
	uint32_t _output_buffers_read_index	 = 0;
	uint32_t _output_buffers_write_index = 0;

	bool _in_paused	 = false;
	bool _out_paused = false;
};

}	// namespace leka
