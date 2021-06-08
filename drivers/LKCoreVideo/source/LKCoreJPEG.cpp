// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreJPEG.h"
#include "LogKit.h"

#include "corevideo_config.h"

namespace leka {

LKCoreJPEG::LKCoreJPEG(LKCoreSTM32HalBase &hal, LKCoreDMA2DBase &dma2d, LKCoreFatFsBase &file)
	: _hal(hal), _dma2d(dma2d), _file(file)
{
	_hjpeg.Instance = JPEG;
}

void LKCoreJPEG::initialize()
{
	JPEG_InitColorTables();
	_hal.HAL_RCC_JPEG_CLK_ENABLE();
	_hal.HAL_JPEG_Init(&_hjpeg);
}

JPEG_ConfTypeDef LKCoreJPEG::getConfig(void)
{
	return _config;
}

JPEG_HandleTypeDef LKCoreJPEG::getHandle(void)
{
	return _hjpeg;
}

JPEG_HandleTypeDef *LKCoreJPEG::getHandlePointer(void)
{
	return &_hjpeg;
}

uint32_t LKCoreJPEG::getWidthOffset(void)
{
	uint32_t width_offset = 0;

	switch (_config.ChromaSubsampling) {
		case JPEG_420_SUBSAMPLING:
			if ((_config.ImageWidth % 16) != 0) {
				width_offset = 16 - (_config.ImageWidth % 16);
			}
			break;
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

void LKCoreJPEG::displayImage(FIL *file)
{
	auto start_time = HAL_GetTick();
	_previous_frame_size = 0;
	
	decodeImageWithPolling();	// TODO: handle errors

	_hal.HAL_JPEG_GetInfo(&_hjpeg, &_config);
	
	_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());

	log_info("Image time : %dms", HAL_GetTick() - start_time);
}

uint32_t findFrameOffset(uint32_t offset, LKCoreFatFsBase &file) 
{
	uint8_t pattern_search_buffer[jpeg::PATTERN_SEARCH_BUFFERSIZE];

	uint32_t index     = offset;
	uint32_t read_size = 0;

	do {
		if (file.getSize() <= (index + 1)) {
			return 0;
		}
		file.seek(index);
		file.read(pattern_search_buffer, jpeg::PATTERN_SEARCH_BUFFERSIZE, &read_size);

		if (read_size != 0) {
			for (uint32_t i = 0; i < (read_size - 1); i++) {
				if ((pattern_search_buffer[i] == jpeg::JPEG_SOI_MARKER_BYTE1) &&
					(pattern_search_buffer[i + 1] == jpeg::JPEG_SOI_MARKER_BYTE0)) {
					return index + i;
				}
			}
			index += (read_size - 1);
		}
	} while (read_size != 0);

	return 0;
}


void LKCoreJPEG::playVideo()
{
	bool is_first_frame	  = true;
	uint32_t frame_index  = 0;
	uint32_t frame_offset = 0;

	do {
		frame_offset = findFrameOffset(frame_offset + _previous_frame_size, _file);
		if (frame_offset != 0) {
			auto start_time = HAL_GetTick();

			_file.seek(frame_offset);
			_previous_frame_size = 0;
			decodeImageWithPolling();

			frame_index++;

			if (is_first_frame) {
				is_first_frame = false;
				HAL_JPEG_GetInfo(&_hjpeg, &_config);
			}
			_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());
			
			log_info("framenb : %d, Frame time %dms", frame_index,HAL_GetTick() - start_time);
		}
	} while (frame_offset != 0);
}


HAL_StatusTypeDef LKCoreJPEG::decodeImageWithPolling(void)
{
	// WARNING: DO NOT REMOVE
	_mcu_block_index = 0;

	// TODO: rely on LKFileSystemKit to handle open/read/close
	uint32_t read_size;
	if (_file.read(_jpeg_input_buffer.data(), _jpeg_input_buffer.size(), &read_size) != FR_OK) {
		return HAL_ERROR;
	}
	_input_file_offset = read_size;

	_hal.HAL_JPEG_Decode(&_hjpeg, 
						_jpeg_input_buffer.data(), read_size,
						_jpeg_output_buffer.data(), _jpeg_output_buffer.size(),
						HAL_MAX_DELAY);
	return HAL_OK;
}

HAL_StatusTypeDef LKCoreJPEG::decodeImageWithDMA(void) 
{

	
	return HAL_OK;
}

void LKCoreJPEG::onErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO: handle errors
}

void LKCoreJPEG::onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
{
	if (info->ChromaSubsampling == JPEG_420_SUBSAMPLING) {
		if ((info->ImageWidth % 16) != 0) info->ImageWidth += (16 - (info->ImageWidth % 16));

		if ((info->ImageHeight % 16) != 0) info->ImageHeight += (16 - (info->ImageHeight % 16));
	}

	if (info->ChromaSubsampling == JPEG_422_SUBSAMPLING) {
		if ((info->ImageWidth % 16) != 0) info->ImageWidth += (16 - (info->ImageWidth % 16));

		if ((info->ImageHeight % 8) != 0) info->ImageHeight += (8 - (info->ImageHeight % 8));
	}

	if (info->ChromaSubsampling == JPEG_444_SUBSAMPLING) {
		if ((info->ImageWidth % 8) != 0) info->ImageWidth += (8 - (info->ImageWidth % 8));

		if ((info->ImageHeight % 8) != 0) info->ImageHeight += (8 - (info->ImageHeight % 8));
	}

	if (JPEG_GetDecodeColorConvertFunc(info, &pConvert_Function, &_mcu_number) != HAL_OK) {
		// TODO: handle errors
	}
}

void LKCoreJPEG::onDataAvailableCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)
{
	// TODO: rely on LKFileSystemKit to handle open/read/close
	if (size != _jpeg_input_buffer.size()) {
		_input_file_offset = _input_file_offset - _jpeg_input_buffer.size() + size;
		_file.seek(_input_file_offset);
	}

	uint32_t read_size;
	if (_file.read(_jpeg_input_buffer.data(), _jpeg_input_buffer.size(), &read_size) == FR_OK) {
		_input_file_offset += read_size;
		_hal.HAL_JPEG_ConfigInputBuffer(hjpeg, _jpeg_input_buffer.data(), read_size);
	} else {
		// TODO: handle error
		log_error("FILE READ ERROR");
	}
	_previous_frame_size += size;
}
void LKCoreJPEG::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t size)
{
	_mcu_block_index +=
		pConvert_Function(pDataOut, (uint8_t *)jpeg::decoded_buffer_address, _mcu_block_index, size, nullptr);

	_hal.HAL_JPEG_ConfigOutputBuffer(hjpeg, _jpeg_output_buffer.data(), _jpeg_output_buffer.size());
}

void LKCoreJPEG::onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)
{
	// TODO: implement flag
}

}	// namespace leka
