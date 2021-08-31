// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreJPEG.hpp"

#include "internal/corevideo_config.h"

using namespace leka;

CoreJPEG::CoreJPEG(interface::STM32Hal &hal, interface::JPEGMode &mode) : _hal(hal), _mode(mode)
{
	_hjpeg.Instance = JPEG;
}

void CoreJPEG::initialize()
{
	__HAL_RCC_JPEG_CLK_ENABLE();
	__HAL_RCC_JPEG_FORCE_RESET();
	__HAL_RCC_JPEG_RELEASE_RESET();

	_hal.HAL_NVIC_SetPriority(JPEG_IRQn, 0x06, 0x0F);
	_hal.HAL_NVIC_EnableIRQ(JPEG_IRQn);

	registerCallbacks();

	JPEG_InitColorTables();
	_hal.HAL_JPEG_Init(&_hjpeg);

	// need to be called again because JPEG_Init resets the callbacks
	registerCallbacks();
}

void displayInformation(JPEGConfig config)
{
	printf("JPEG file configuration:\n");

	printf("ColorSpace: %ld\n", config.ColorSpace);
	printf("ChromaSubsampling: %ld [0 -> 4:4:4 | 1 -> 4:2:0 | 2 -> 4:2:2]\n", config.ChromaSubsampling);
	printf("ImageHeight: %ld\n", config.ImageHeight);
	printf("ImageWidth: %ld\n", config.ImageWidth);
	printf("ImageQuality: %ld\n\n", config.ImageQuality);

	//   uint32_t ColorSpace;               /*!< Image Color space : gray-scale, YCBCR, RGB or CMYK
	//                                            This parameter can be a value of @ref JPEG_ColorSpace */
	//   uint32_t ChromaSubsampling;        /*!< Chroma Subsampling in case of YCBCR or CMYK color space, 0-> 4:4:4 ,
	//   1-> 4:2:2, 2 -> 4:1:1, 3 -> 4:2:0        This parameter can be a value of @ref JPEG_ChromaSubsampling */
	//   uint32_t ImageHeight;              /*!< Image height : number of lines */
	//   uint32_t ImageWidth;               /*!< Image width : number of pixels per line */
	//   uint32_t ImageQuality;             /*!< Quality of the JPEG encoding : from 1 to 100 */
}

auto CoreJPEG::getConfig() -> JPEGConfig
{
	JPEGConfig config;
	_hal.HAL_JPEG_GetInfo(&_hjpeg, &config);

	displayInformation(config);

	config.initialized = true;
	return config;
}

auto CoreJPEG::getHandle() -> JPEG_HandleTypeDef &
{
	return _hjpeg;
}

void CoreJPEG::registerCallbacks()
{
	static CoreJPEG *self;
	self = this;

	_hal.HAL_JPEG_RegisterInfoReadyCallback(&_hjpeg, [](JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) {
		self->_mode.onInfoReadyCallback(hjpeg, info);
	});

	_hal.HAL_JPEG_RegisterGetDataCallback(&_hjpeg, [](JPEG_HandleTypeDef *hjpeg, uint32_t decoded_datasize) {
		self->_mode.onGetDataCallback(hjpeg, decoded_datasize);
	});

	_hal.HAL_JPEG_RegisterDataReadyCallback(&_hjpeg,
											[](JPEG_HandleTypeDef *hjpeg, uint8_t *output_data, uint32_t datasize) {
												self->_mode.onDataReadyCallback(hjpeg, output_data, datasize);
											});

	_hal.HAL_JPEG_RegisterCallback(&_hjpeg, HAL_JPEG_DECODE_CPLT_CB_ID,
								   [](JPEG_HandleTypeDef *hjpeg) { self->_mode.onDecodeCompleteCallback(hjpeg); });

	_hal.HAL_JPEG_RegisterCallback(&_hjpeg, HAL_JPEG_ERROR_CB_ID,
								   [](JPEG_HandleTypeDef *hjpeg) { self->_mode.onErrorCallback(hjpeg); });

	_hal.HAL_JPEG_RegisterCallback(&_hjpeg, HAL_JPEG_MSPINIT_CB_ID,
								   [](JPEG_HandleTypeDef *hjpeg) { self->_mode.onMspInitCallback(hjpeg); });
}

auto CoreJPEG::decodeImage(interface::File &file) -> std::uint32_t
{
	return _mode.decodeImage(&_hjpeg, file);
}

auto CoreJPEG::findFrameOffset(interface::File &file, uint32_t offset) -> uint32_t
{
	std::array<uint8_t, 512> pattern_search_buffer;

	size_t file_size   = file.size();
	uint32_t index	   = offset;
	uint32_t read_size = 0;

	do {
		if (file_size <= (index + 1)) {
			return 0;
		}
		file.seek(index, SEEK_SET);
		read_size = file.read(pattern_search_buffer.data(), pattern_search_buffer.size());

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

auto JPEGConfig::getWidthOffset() const -> uint32_t
{
	uint32_t width_offset = 0;

	switch (ChromaSubsampling) {
		case JPEG_420_SUBSAMPLING:
			if ((ImageWidth % 16) != 0) {
				width_offset = 16 - (ImageWidth % 16);
			}
			break;
		case JPEG_422_SUBSAMPLING:
			if ((ImageWidth % 16) != 0) {
				width_offset = 16 - (ImageWidth % 16);
			}
			break;
		case JPEG_444_SUBSAMPLING:
			if ((ImageWidth % 8) != 0) {
				width_offset = (ImageWidth % 8);
			}
			break;
		default:
			width_offset = 0;
			break;
	}

	return width_offset;
}
