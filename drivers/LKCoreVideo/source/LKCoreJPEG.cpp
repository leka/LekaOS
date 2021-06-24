// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreJPEG.h"
#include "LogKit.h"

#include "corevideo_config.h"
#include "decode_dma.h"

void HAL_JPEG_MspInit(JPEG_HandleTypeDef *hjpeg)
{
	static DMA_HandleTypeDef   hdmaIn;
	static DMA_HandleTypeDef   hdmaOut;

	/* Enable JPEG clock */
	__HAL_RCC_JPEG_CLK_ENABLE();

	/* Enable DMA clock */
	__HAL_RCC_DMA2_CLK_ENABLE();

	HAL_NVIC_SetPriority(JPEG_IRQn, 0x06, 0x0F);
	HAL_NVIC_EnableIRQ(JPEG_IRQn);

	/* Input DMA */
	/* Set the parameters to be configured */
	hdmaIn.Init.Channel = DMA_CHANNEL_9;
	hdmaIn.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdmaIn.Init.PeriphInc = DMA_PINC_DISABLE;
	hdmaIn.Init.MemInc = DMA_MINC_ENABLE;
	hdmaIn.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdmaIn.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdmaIn.Init.Mode = DMA_NORMAL;
	hdmaIn.Init.Priority = DMA_PRIORITY_HIGH;
	hdmaIn.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdmaIn.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdmaIn.Init.MemBurst = DMA_MBURST_INC4;
	hdmaIn.Init.PeriphBurst = DMA_PBURST_INC4;

	hdmaIn.Instance = DMA2_Stream3;

	/* Associate the DMA handle */
	__HAL_LINKDMA(hjpeg, hdmain, hdmaIn);

	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0x07, 0x0F);
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

	/* DeInitialize the DMA Stream */
	HAL_DMA_DeInit(&hdmaIn);
	/* Initialize the DMA stream */
	HAL_DMA_Init(&hdmaIn);


	/* Output DMA */
	/* Set the parameters to be configured */
	hdmaOut.Init.Channel = DMA_CHANNEL_9;
	hdmaOut.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdmaOut.Init.PeriphInc = DMA_PINC_DISABLE;
	hdmaOut.Init.MemInc = DMA_MINC_ENABLE;
	hdmaOut.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdmaOut.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdmaOut.Init.Mode = DMA_NORMAL;
	hdmaOut.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdmaOut.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdmaOut.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdmaOut.Init.MemBurst = DMA_MBURST_INC4;
	hdmaOut.Init.PeriphBurst = DMA_PBURST_INC4;


	hdmaOut.Instance = DMA2_Stream4;
	/* DeInitialize the DMA Stream */
	HAL_DMA_DeInit(&hdmaOut);
	/* Initialize the DMA stream */
	HAL_DMA_Init(&hdmaOut);

	/* Associate the DMA handle */
	__HAL_LINKDMA(hjpeg, hdmaout, hdmaOut);

	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0x07, 0x0F);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

}

namespace leka {

LKCoreJPEG::LKCoreJPEG(LKCoreSTM32HalBase &hal, LKCoreDMA2DBase &dma2d, LKCoreFatFsBase &file, std::unique_ptr<Mode> mode)
	: _hal(hal), _dma2d(dma2d), _file(file), _mode(std::move(mode))
{
	_hjpeg.Instance = JPEG;
}

void LKCoreJPEG::initialize()
{
	JPEG_InitColorTables();
	_hal.HAL_RCC_JPEG_CLK_ENABLE();
	_hal.HAL_JPEG_Init(&_hjpeg);

	registerCallbacks();
}

JPEG_ConfTypeDef& LKCoreJPEG::getConfig(void)
{
	return _config;
}

JPEG_HandleTypeDef& LKCoreJPEG::getHandle(void)
{
	return _hjpeg;
}

JPEG_HandleTypeDef* LKCoreJPEG::getHandlePointer(void)
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
	registerCallbacks();

	uint32_t frame_index = 0;
	uint32_t frame_size = 0;
	uint32_t frame_offset = findFrameOffset(0, _file);

	while (frame_offset != 0) {
		auto start_time = HAL_GetTick();

		_file.seek(frame_offset);

		frame_size = decodeImage();
		// if first frame, get file info
		if (frame_index == 0)
			HAL_JPEG_GetInfo(&_hjpeg, &_config);
		frame_index += 1;

		_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());

		// get next frame offset
		frame_offset = findFrameOffset(frame_offset+frame_size, _file);

		auto dt = HAL_GetTick() - start_time;
		log_info("%dms = %f fps", dt, 1000.f/dt);
	}
}

auto LKCoreJPEG::decodeImage(void) -> uint32_t
{
	return _mode->decodeImage(&_hjpeg, _file.getPointer());
}

void LKCoreJPEG::registerCallbacks(void) {
	static auto* self = this;

	HAL_JPEG_RegisterInfoReadyCallback(
		&self->getHandle(),
		[](JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) {
			self->_mode->onInfoReadyCallback(hjpeg, info);
		}
	);

	HAL_JPEG_RegisterGetDataCallback(
		&self->getHandle(),
		[](JPEG_HandleTypeDef *hjpeg, uint32_t size) {
			self->_mode->onGetDataCallback(hjpeg, size);
		}
	);

	HAL_JPEG_RegisterDataReadyCallback(
		&self->getHandle(),
		[](JPEG_HandleTypeDef *hjpeg, uint8_t *pDataOut, uint32_t size) {
			self->_mode->onDataReadyCallback(hjpeg, pDataOut, size);
		}
	);

	HAL_JPEG_RegisterCallback(
		&self->getHandle(), HAL_JPEG_DECODE_CPLT_CB_ID,
		[](JPEG_HandleTypeDef *hjpeg) {
			self->_mode->onDecodeCompleteCallback(hjpeg);
		}
	);

	HAL_JPEG_RegisterCallback(
		&self->getHandle(), HAL_JPEG_ERROR_CB_ID,
		[](JPEG_HandleTypeDef *hjpeg) {
			self->_mode->onErrorCallback(hjpeg);
		}
	);
}


void LKCoreJPEG::Mode::onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info)
{
	switch (info->ChromaSubsampling) {
		case JPEG_420_SUBSAMPLING:
			if((info->ImageWidth % 16) != 0)
				info->ImageWidth += (16 - (info->ImageWidth % 16));
			if((info->ImageHeight % 16) != 0)
				info->ImageHeight += (16 - (info->ImageHeight % 16));
			break;

		case JPEG_422_SUBSAMPLING:
			if((info->ImageWidth % 16) != 0)
				info->ImageWidth += (16 - (info->ImageWidth % 16));
			if((info->ImageHeight % 8) != 0)
				info->ImageHeight += (8 - (info->ImageHeight % 8));
			break;

		case JPEG_444_SUBSAMPLING:
			if((info->ImageWidth % 8) != 0)
				info->ImageWidth += (8 - (info->ImageWidth % 8));
			if((info->ImageHeight % 8) != 0)
				info->ImageHeight += (8 - (info->ImageHeight % 8));
			break;
	}

	if(JPEG_GetDecodeColorConvertFunc(info, &pConvert_Function, &_mcu_number) != HAL_OK)
		while(1);
}

void LKCoreJPEG::Mode::onErrorCallback(JPEG_HandleTypeDef *hjpeg)
{
	while(1);
}

void LKCoreJPEG::Mode::onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg)
{
	_hw_decode_ended = true;
}

auto LKCoreJPEG::PollingMode::decodeImage(JPEG_HandleTypeDef *hjpeg, FIL* file) -> uint32_t
{
	_file = file;
	// WARNING: DO NOT REMOVE
	_mcu_block_index = 0;
	_previous_image_size = 0;

	// TODO: rely on LKFileSystemKit to handle open/read/close
	unsigned read_size;
	if (f_read(file, _jpeg_input_buffer.data(), _jpeg_input_buffer.size(), &read_size) != FR_OK)
		return HAL_ERROR;

	_input_file_offset = read_size;

	HAL_JPEG_Decode(hjpeg,
					_jpeg_input_buffer.data(), read_size,
					_jpeg_output_buffer.data(), _jpeg_output_buffer.size(),
					HAL_MAX_DELAY);
	return _previous_image_size;
}

void LKCoreJPEG::PollingMode::onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)
{
	// TODO: rely on LKFileSystemKit to handle open/read/close
	if (size != _jpeg_input_buffer.size()) {
		_input_file_offset = _input_file_offset - _jpeg_input_buffer.size() + size;
		f_lseek(_file, _input_file_offset);
	}

	unsigned read_size;
	if (f_read(_file, _jpeg_input_buffer.data(), _jpeg_input_buffer.size(), &read_size) == FR_OK) {
		_input_file_offset += read_size;
		HAL_JPEG_ConfigInputBuffer(hjpeg, _jpeg_input_buffer.data(), read_size);
	}
	else {
		while(1); // TODO: handle error
	}
	_previous_image_size += size;
}

void LKCoreJPEG::PollingMode::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size)
{
	_mcu_block_index += pConvert_Function(output_buffer, (uint8_t*)jpeg::decoded_buffer_address, _mcu_block_index, size, nullptr);

	HAL_JPEG_ConfigOutputBuffer(hjpeg, _jpeg_output_buffer.data(), _jpeg_output_buffer.size());
}

auto LKCoreJPEG::DMAMode::decodeImage(JPEG_HandleTypeDef *hjpeg, FIL* file) -> uint32_t
{
	uint32_t i;

	_previous_image_size = 0;

	_mcu_number = 0;
	_mcu_block_index = 0;
	_hw_decode_ended = false;

	_out_read_index = 0;
	_out_write_index = 0;
	_out_paused = false;

	_in_read_index = 0;
	_in_write_index = 0;
	_in_paused = 0;

	_jpeg_out_buffers[0].state = BufferState::Empty;
	_jpeg_out_buffers[0].size = 0;

	_jpeg_out_buffers[1].state = BufferState::Empty;
	_jpeg_out_buffers[1].size = 0;

	/* Read from JPG file and fill input buffers */
	for(i = 0; i < 2; i++)
	{
		if(f_read (file, _jpeg_in_buffers[i].array.data() , jpeg::dma::chunk_size_in, &_jpeg_in_buffers[i].size) == FR_OK) {
			_jpeg_in_buffers[i].state = BufferState::Full;
		}
		else {
			// TODO : handle error
			while(1);
		}
	}
	// Start JPEG decoding with DMA method
	HAL_JPEG_Decode_DMA(hjpeg ,_jpeg_in_buffers[0].array.data() ,_jpeg_in_buffers[0].size, _jpeg_out_buffers[0].array.data(), jpeg::dma::chunk_size_out);

	bool jpeg_decode_end = false;
	do {
		decoderInputHandler(hjpeg, file);
		jpeg_decode_end = decoderOutputHandler(hjpeg);
	} while(jpeg_decode_end == false);

	return _previous_image_size;
}

void LKCoreJPEG::DMAMode::onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size)
{
	if(size == _jpeg_in_buffers[_in_read_index].size)
	{
		_jpeg_in_buffers[_in_read_index].state = BufferState::Empty;
		_jpeg_in_buffers[_in_read_index].size = 0;

		_in_read_index++;
		if(_in_read_index >= 2)
		{
			_in_read_index = 0;
		}

		if(_jpeg_in_buffers[_in_read_index].state == BufferState::Empty)
		{
			HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_INPUT);
			_in_paused = true;
		}
		else
		{
			HAL_JPEG_ConfigInputBuffer(hjpeg,_jpeg_in_buffers[_in_read_index].array.data(), _jpeg_in_buffers[_in_read_index].size);
		}
	}
	else
	{
		HAL_JPEG_ConfigInputBuffer(hjpeg,_jpeg_in_buffers[_in_read_index].array.data() + size, _jpeg_in_buffers[_in_read_index].size - size);
	}
	_previous_image_size += size;
	//decode_dma::GetDataCallback(hjpeg, size);
}

void LKCoreJPEG::DMAMode::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size)
{
	_jpeg_out_buffers[_out_write_index].state = BufferState::Full;
	_jpeg_out_buffers[_out_write_index].size = size;

	_out_write_index++;
	if(_out_write_index >= 2)
	{
		_out_write_index = 0;
	}

	if(_jpeg_out_buffers[_out_write_index].state != BufferState::Empty) {
		HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
		_out_paused = true;
	}
	HAL_JPEG_ConfigOutputBuffer(hjpeg, _jpeg_out_buffers[_out_write_index].array.data(), jpeg::dma::chunk_size_out);
	//decode_dma::DataReadyCallback(hjpeg, output_buffer, size);
}

void LKCoreJPEG::DMAMode::decoderInputHandler(JPEG_HandleTypeDef *hjpeg, FIL *file) {
	if(_jpeg_in_buffers[_in_write_index].state == BufferState::Empty)
	{
		if(f_read(file, _jpeg_in_buffers[_in_write_index].array.data() , jpeg::dma::chunk_size_in, &_jpeg_in_buffers[_in_write_index].size) == FR_OK)
		{
			_jpeg_in_buffers[_in_write_index].state = BufferState::Full;
		}
		else
		{
			while(1); // TODO : handle error
		}

		if((_in_paused == true) && (_in_write_index == _in_read_index))
		{
			_in_paused = false;
			HAL_JPEG_ConfigInputBuffer(hjpeg, _jpeg_in_buffers[_in_read_index].array.data(), _jpeg_in_buffers[_in_read_index].size);
			HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_INPUT);
		}

		_in_write_index++;
		if(_in_write_index >= 2)
		{
			_in_write_index = 0;
		}
	}
}

bool LKCoreJPEG::DMAMode::decoderOutputHandler(JPEG_HandleTypeDef *hjpeg) {
	if(_jpeg_out_buffers[_out_read_index].state == BufferState::Full)
	{
		_mcu_block_index += pConvert_Function(_jpeg_out_buffers[_out_read_index].array.data(), (uint8_t *)jpeg::decoded_buffer_address, _mcu_block_index, _jpeg_out_buffers[_out_read_index].size, NULL);

		_jpeg_out_buffers[_out_read_index].state = BufferState::Empty;
		_jpeg_out_buffers[_out_read_index].size = 0;

		_out_read_index++;
		if(_out_read_index >= 2)
		{
			_out_read_index = 0;
		}

		if(_mcu_block_index == _mcu_number)
		{
			return true;
		}
	}
	else if((_out_paused == true)
			&& (_jpeg_out_buffers[_out_write_index].state == BufferState::Empty)
			&& (_jpeg_out_buffers[_out_read_index].state == BufferState::Empty))
	{
		_out_paused = false;
		HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
	}
	return false;
}

auto LKCoreJPEG::ST_DMAMode::decodeImage(JPEG_HandleTypeDef *hjpeg, FIL *file) -> uint32_t {
	decode_dma::JPEG_Decode_DMA(hjpeg, file, jpeg::decoded_buffer_address);

	int process_ended = 0;

	do {
		decode_dma::JPEG_InputHandler(hjpeg);
		process_ended = decode_dma::JPEG_OutputHandler(hjpeg);
	} while (process_ended == 0);

	return decode_dma::Previous_FrameSize;
}

void LKCoreJPEG::ST_DMAMode::onInfoReadyCallback(JPEG_HandleTypeDef *hjpeg, JPEG_ConfTypeDef *info) {
	decode_dma::InfoReadyCallback(hjpeg, info);
}

void LKCoreJPEG::ST_DMAMode::onDecodeCompleteCallback(JPEG_HandleTypeDef *hjpeg) {
	decode_dma::DecodeCpltCallback(hjpeg);
}

void LKCoreJPEG::ST_DMAMode::onErrorCallback(JPEG_HandleTypeDef *hjpeg) {
	decode_dma::ErrorCallback(hjpeg);
}

void LKCoreJPEG::ST_DMAMode::onGetDataCallback(JPEG_HandleTypeDef *hjpeg, uint32_t size) {
	decode_dma::GetDataCallback(hjpeg, size);
}

void LKCoreJPEG::ST_DMAMode::onDataReadyCallback(JPEG_HandleTypeDef *hjpeg, uint8_t *output_buffer, uint32_t size) {
	decode_dma::DataReadyCallback(hjpeg, output_buffer, size);
}

}	// namespace leka
