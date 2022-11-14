// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEGModePolling.hpp"

using namespace leka;

void CoreJPEGModePolling::onMspInitCallback(JPEG_HandleTypeDef *hjpeg)
{
	// nothing to do
}

void CoreJPEGModePolling::onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
{
	if (info->ChromaSubsampling == JPEG_420_SUBSAMPLING) {
		if ((info->ImageWidth % 16) != 0) {
			info->ImageWidth += (16 - (info->ImageWidth % 16));
		}

		if ((info->ImageHeight % 16) != 0) {
			info->ImageHeight += (16 - (info->ImageHeight % 16));
		}
	}

	if (info->ChromaSubsampling == JPEG_422_SUBSAMPLING) {
		if ((info->ImageWidth % 16) != 0) {
			info->ImageWidth += (16 - (info->ImageWidth % 16));
		}

		if ((info->ImageHeight % 8) != 0) {
			info->ImageHeight += (8 - (info->ImageHeight % 8));
		}
	}

	if (info->ChromaSubsampling == JPEG_444_SUBSAMPLING) {
		if ((info->ImageWidth % 8) != 0) {
			info->ImageWidth += (8 - (info->ImageWidth % 8));
		}

		if ((info->ImageHeight % 8) != 0) {
			info->ImageHeight += (8 - (info->ImageHeight % 8));
		}
	}

	if (JPEG_GetDecodeColorConvertFunc(info, &pConvert_Function, &_mcu_number) != HAL_OK) {
		// TODO(@yann): handle errors
	}
}

void CoreJPEGModePolling::onErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO(@yann): handle errors
}

void CoreJPEGModePolling::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size)
{
	_mcu_block_index += pConvert_Function(output_buffer, reinterpret_cast<uint8_t *>(jpeg::decoded_buffer_address),
										  _mcu_block_index, size, nullptr);

	_hal.HAL_JPEG_ConfigOutputBuffer(hjpeg, _mcu_data_output_buffer.data(), leka::jpeg::output_chunk_size);
}

void CoreJPEGModePolling::onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)
{
	if (size != _jpeg_input_buffer.size) {
		_input_file_offset = _input_file_offset - _jpeg_input_buffer.size + size;
		_file->seek(_input_file_offset, SEEK_SET);
	}

	if (_jpeg_input_buffer.size = _file->read(_jpeg_input_buffer.data, leka::jpeg::input_chunk_size);
		_jpeg_input_buffer.size != 0) {
		_input_file_offset += _jpeg_input_buffer.size;
		_hal.HAL_JPEG_ConfigInputBuffer(hjpeg, _jpeg_input_buffer.data, _jpeg_input_buffer.size);
	} else {
		// TODO(@yann): handle error
	}

	_image_size += _jpeg_input_buffer.size;
}

void CoreJPEGModePolling::onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO(@yann): implement flag
}

auto CoreJPEGModePolling::decode(JPEG_HandleTypeDef *hjpeg, interface::File &file) -> std::size_t
{
	_file = &file;

	// WARNING: DO NOT REMOVE
	_image_size		 = 0;
	_mcu_block_index = 0;

	if (_jpeg_input_buffer.size = _file->read(_jpeg_input_buffer.data, leka::jpeg::input_chunk_size);
		_jpeg_input_buffer.size == 0) {
		return HAL_ERROR;
	}

	_input_file_offset = _jpeg_input_buffer.size;

	_hal.HAL_JPEG_Decode(hjpeg, _jpeg_input_buffer.data, _jpeg_input_buffer.size, _mcu_data_output_buffer.data(),
						 leka::jpeg::output_chunk_size, HAL_MAX_DELAY);

	return _image_size;
}
