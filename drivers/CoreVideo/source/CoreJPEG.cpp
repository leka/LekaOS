// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEG.hpp"

#include "internal/corevideo_config.h"

namespace leka {

CoreJPEG::CoreJPEG(CoreSTM32HalBase &hal, interface::DMA2DBase &dma2d, interface::FatFs &file)
	: _hal(hal), _dma2d(dma2d), _file(file)
{
	_hjpeg.Instance = JPEG;
}

void CoreJPEG::initialize()
{
	JPEG_InitColorTables();
	_hal.HAL_RCC_JPEG_CLK_ENABLE();
	_hal.HAL_JPEG_Init(&_hjpeg);
}

auto CoreJPEG::getConfig() -> JPEG_ConfTypeDef
{
	return _config;
}

auto CoreJPEG::getHandle() -> JPEG_HandleTypeDef
{
	return _hjpeg;
}

auto CoreJPEG::getHandlePointer() -> JPEG_HandleTypeDef *
{
	return &_hjpeg;
}

auto CoreJPEG::getWidthOffset() -> uint32_t
{
	uint32_t width_offset = 0;

	switch (_config.ChromaSubsampling) {
		case JPEG_420_SUBSAMPLING:
		case JPEG_422_SUBSAMPLING:
			if ((_config.ImageWidth % 16) != 0) {
				width_offset = 16 - (_config.ImageWidth % 16);
			}
			break;
		case JPEG_444_SUBSAMPLING:
			if ((_config.ImageWidth % 8) != 0) {
				width_offset = (_config.ImageWidth % 8);
			}
			break;
		default:
			width_offset = 0;
			break;
	}

	return width_offset;
}

void CoreJPEG::displayImage(FIL *file)
{
	decodeImageWithPolling();	// TODO(@yann): handle errors

	_hal.HAL_JPEG_GetInfo(&_hjpeg, &_config);

	_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());
}

auto CoreJPEG::decodeImageWithPolling() -> HAL_StatusTypeDef
{
	// WARNING: DO NOT REMOVE
	_mcu_block_index = 0;

	// TODO(@yann): rely on LKFileSystemKit to handle open/read/close
	if (_file.read(_jpeg_input_buffer.data, leka::jpeg::input_data_buffer_size, &_jpeg_input_buffer.size) != FR_OK) {
		return HAL_ERROR;
	}

	_input_file_offset = _jpeg_input_buffer.size;

	_hal.HAL_JPEG_Decode(&_hjpeg, _jpeg_input_buffer.data, _jpeg_input_buffer.size, _mcu_data_output_buffer.data(),
						 leka::jpeg::mcu::output_data_buffer_size, HAL_MAX_DELAY);

	return HAL_OK;
}

void CoreJPEG::onErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO(@yann): handle errors
}

void CoreJPEG::onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
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

void CoreJPEG::onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)
{
	// TODO(@yann): rely on LKFileSystemKit to handle open/read/close
	if (size != _jpeg_input_buffer.size) {
		_input_file_offset = _input_file_offset - _jpeg_input_buffer.size + size;
		_file.seek(_input_file_offset);
	}

	if (_file.read(_jpeg_input_buffer.data, leka::jpeg::input_data_buffer_size, &_jpeg_input_buffer.size) == FR_OK) {
		_input_file_offset += _jpeg_input_buffer.size;
		_hal.HAL_JPEG_ConfigInputBuffer(hjpeg, _jpeg_input_buffer.data, _jpeg_input_buffer.size);
	} else {
		// TODO(@yann): handle error
	}
}
void CoreJPEG::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size)
{
	_mcu_block_index += pConvert_Function(output_buffer, reinterpret_cast<uint8_t *>(jpeg::decoded_buffer_address),
										  _mcu_block_index, size, nullptr);

	_hal.HAL_JPEG_ConfigOutputBuffer(hjpeg, _mcu_data_output_buffer.data(), leka::jpeg::mcu::output_data_buffer_size);
}

void CoreJPEG::onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO(@yann): implement flag
}

}	// namespace leka
