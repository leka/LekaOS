// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "LKCoreJPEG.h"
#include "LogKit.h"

#include "corevideo_config.h"

void HAL_JPEG_MspInit(JPEG_HandleTypeDef* hjpeg)
{
	static DMA_HandleTypeDef hdmaIn;
	static DMA_HandleTypeDef hdmaOut;

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

	hdmaIn.Instance = DMA2_Stream0;

	/* DeInitialize the DMA Stream */
	HAL_DMA_DeInit(&hdmaIn);
	/* Initialize the DMA stream */
	HAL_DMA_Init(&hdmaIn);

	/* Associate the DMA handle */
	__HAL_LINKDMA(hjpeg, hdmain, hdmaIn);

	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0x07, 0x0F);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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

	hdmaOut.Instance = DMA2_Stream1;
	/* DeInitialize the DMA Stream */
	HAL_DMA_DeInit(&hdmaOut);
	/* Initialize the DMA stream */
	HAL_DMA_Init(&hdmaOut);

	/* Associate the DMA handle */
	__HAL_LINKDMA(hjpeg, hdmaout, hdmaOut);

	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0x07, 0x0F);
	HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

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

auto LKCoreJPEG::getConfig(void) -> JPEG_ConfTypeDef&
{
	return _config;
}

auto LKCoreJPEG::getHandle(void) -> JPEG_HandleTypeDef&
{
	return _hjpeg;
}

auto LKCoreJPEG::getHandlePointer(void) -> JPEG_HandleTypeDef*
{
	return &_hjpeg;
}

auto LKCoreJPEG::getWidthOffset(void) -> uint32_t
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

auto findFrameOffset(uint32_t offset, LKCoreFatFsBase& file) -> uint32_t
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
		_file.seek(frame_offset);

		auto start_time = HAL_GetTick();
		frame_size = decodeImage();

		// if first frame, get file info
		if (frame_index == 0)
			HAL_JPEG_GetInfo(&_hjpeg, &_config);
		frame_index += 1;

		_dma2d.transferImage(_config.ImageWidth, _config.ImageHeight, getWidthOffset());

		// get next frame offset
		frame_offset = findFrameOffset(frame_offset+frame_size+4, _file);
		auto dt = HAL_GetTick() - start_time;
		log_info("%dms = %f fps", dt, 1000.f/dt);
	}
}

auto LKCoreJPEG::decodeImage(void) -> uint32_t
{
	return _mode->decodeImage(&_hjpeg, _file.getPointer());
}

void LKCoreJPEG::registerCallbacks(void)
{
	static auto* self = this;

	HAL_JPEG_RegisterInfoReadyCallback(
		&self->getHandle(),
		[](JPEG_HandleTypeDef* hjpeg, JPEG_ConfTypeDef *info) {
			self->_mode->onInfoReadyCallback(hjpeg, info);
		}
	);

	HAL_JPEG_RegisterGetDataCallback(
		&self->getHandle(),
		[](JPEG_HandleTypeDef* hjpeg, uint32_t size) {
			self->_mode->onGetDataCallback(hjpeg, size);
		}
	);

	HAL_JPEG_RegisterDataReadyCallback(
		&self->getHandle(),
		[](JPEG_HandleTypeDef* hjpeg, uint8_t *pDataOut, uint32_t size) {
			self->_mode->onDataReadyCallback(hjpeg, pDataOut, size);
		}
	);

	HAL_JPEG_RegisterCallback(
		&self->getHandle(), HAL_JPEG_DECODE_CPLT_CB_ID,
		[](JPEG_HandleTypeDef* hjpeg) {
			self->_mode->onDecodeCompleteCallback(hjpeg);
		}
	);

	HAL_JPEG_RegisterCallback(
		&self->getHandle(), HAL_JPEG_ERROR_CB_ID,
		[](JPEG_HandleTypeDef* hjpeg) {
			self->_mode->onErrorCallback(hjpeg);
		}
	);
}


void LKCoreJPEG::Mode::onInfoReadyCallback(JPEG_HandleTypeDef* hjpeg, JPEG_ConfTypeDef* info)
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

void LKCoreJPEG::Mode::onErrorCallback(JPEG_HandleTypeDef* hjpeg)
{
	while(1);
}

void LKCoreJPEG::Mode::onDecodeCompleteCallback(JPEG_HandleTypeDef* hjpeg)
{
	_hw_decode_ended = true;
}

auto LKCoreJPEG::PollingMode::decodeImage(JPEG_HandleTypeDef* hjpeg, FIL* file) -> uint32_t
{
	_file = file;
	_mcu_block_index = 0;
	_previous_image_size = 0;

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

void LKCoreJPEG::PollingMode::onGetDataCallback(JPEG_HandleTypeDef* hjpeg, uint32_t decoded_datasize)
{
	if (decoded_datasize != _jpeg_input_buffer.size()) {
		_input_file_offset = _input_file_offset - _jpeg_input_buffer.size() + decoded_datasize;
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
	_previous_image_size += decoded_datasize;
}

void LKCoreJPEG::PollingMode::onDataReadyCallback(JPEG_HandleTypeDef* hjpeg, uint8_t* output_data, uint32_t output_datasize)
{
	_mcu_block_index += pConvert_Function(
							output_data, (uint8_t*)jpeg::decoded_buffer_address,
							_mcu_block_index, output_datasize,
							nullptr
						);

	HAL_JPEG_ConfigOutputBuffer(hjpeg, _jpeg_output_buffer.data(), _jpeg_output_buffer.size());
}

auto LKCoreJPEG::DMAMode::decodeImage(JPEG_HandleTypeDef* hjpeg, FIL* file) -> uint32_t
{
	static uint8_t BIG_CHUNGUS_OF_MEMORY_IN[jpeg::chunk_size_in * jpeg::in_buffers_nb];
	static uint8_t BIG_CHUNGUS_OF_MEMORY_OUT[jpeg::chunk_size_out * jpeg::out_buffers_nb];
	
	_previous_image_size = 0;

	_mcu_number = 0;
	_mcu_block_index = 0;
	_hw_decode_ended = false;

	_out_read_index = 0;
	_out_write_index = 0;
	_out_paused = false;

	_in_read_index = 0;
	_in_write_index = 0;
	_in_paused = false;

	// initialize memory chungus
	uint32_t i = 0;
	for (auto& buffer : _in_buffers) {
		buffer.state = Buffer::State::Empty;
		buffer.datasize = 0;
		buffer.data = BIG_CHUNGUS_OF_MEMORY_IN + i * jpeg::chunk_size_in;
		i += 1;
	}
	i = 0;
	for (auto& buffer : _out_buffers) {
		buffer.state = Buffer::State::Empty;
		buffer.datasize = 0;
		buffer.data = BIG_CHUNGUS_OF_MEMORY_OUT + i * jpeg::chunk_size_out;
		i += 1;
	}

	// read file and fill input buffers
	for (auto& buffer : _in_buffers) {
		if(f_read(file, buffer.data, jpeg::chunk_size_in, &buffer.datasize) == FR_OK)
			buffer.state = Buffer::State::Full;
	}

	// start JPEG decoding with DMA method
	HAL_JPEG_Decode_DMA(hjpeg, _in_buffers[0].data, _in_buffers[0].datasize, _out_buffers[0].data, jpeg::chunk_size_out);

	// loop until decode process ends
	bool process_ended = false;
	unsigned in_time = 0;
	unsigned out_time = 0;
	do {
		unsigned start = HAL_GetTick();
		decoderInputHandler(hjpeg, file);
		in_time += HAL_GetTick() - start;

		start = HAL_GetTick();
		process_ended = decoderOutputHandler(hjpeg);
		out_time += HAL_GetTick() - start;
	} while (process_ended == false);

	return _previous_image_size;
}

void LKCoreJPEG::DMAMode::onGetDataCallback(JPEG_HandleTypeDef* hjpeg, uint32_t decoded_datasize)
{
	auto& read_buffer = _in_buffers[_in_read_index];
	auto& next_read_buffer = _in_buffers[(_in_read_index + 1) % jpeg::in_buffers_nb];

	if(decoded_datasize == read_buffer.datasize) {
		read_buffer.state = Buffer::State::Empty;
		read_buffer.datasize = 0;
	
		_in_read_index = (_in_read_index + 1) % jpeg::in_buffers_nb;

		if(next_read_buffer.state == Buffer::State::Empty) {
			_in_paused = true;
			HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_INPUT);
		}
		else {
			HAL_JPEG_ConfigInputBuffer(hjpeg, next_read_buffer.data, next_read_buffer.datasize);
		}
	}
	else {
		HAL_JPEG_ConfigInputBuffer(hjpeg, next_read_buffer.data + decoded_datasize, next_read_buffer.datasize - decoded_datasize);
	}
	_previous_image_size += decoded_datasize;
}

void LKCoreJPEG::DMAMode::onDataReadyCallback(JPEG_HandleTypeDef* hjpeg, uint8_t* output_data, uint32_t output_datasize)
{
	auto& write_buffer = _out_buffers[_out_write_index];
	auto& next_write_buffer = _out_buffers[(_out_write_index + 1) % jpeg::out_buffers_nb];

	write_buffer.state = Buffer::State::Full;
	write_buffer.datasize = output_datasize;

	_out_write_index = (_out_write_index + 1) % jpeg::out_buffers_nb;

	if(next_write_buffer.state != Buffer::State::Empty) {
		_out_paused = true;
		HAL_JPEG_Pause(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
	}
	HAL_JPEG_ConfigOutputBuffer(hjpeg, next_write_buffer.data, jpeg::chunk_size_out); 
}

void LKCoreJPEG::DMAMode::decoderInputHandler(JPEG_HandleTypeDef* hjpeg, FIL* file)
{
	auto& write_buffer = _in_buffers[_in_write_index];
	auto& read_buffer = _in_buffers[_in_read_index];
	
	if(write_buffer.state == Buffer::State::Empty) {
		if(f_read(file, write_buffer.data, jpeg::chunk_size_in, &write_buffer.datasize) == FR_OK)
			write_buffer.state = Buffer::State::Full;
		else
			while(1);

		if(_in_paused && _in_write_index == _in_read_index) {
			_in_paused = false;
			HAL_JPEG_ConfigInputBuffer(hjpeg, read_buffer.data, read_buffer.datasize);
			HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_INPUT); 
		}

		_in_write_index = (_in_write_index + 1) % jpeg::in_buffers_nb;
	}
}

bool LKCoreJPEG::DMAMode::decoderOutputHandler(JPEG_HandleTypeDef* hjpeg)
{
	uint32_t converted_data_count;
	auto& read_buffer = _out_buffers[_out_read_index];
	auto& write_buffer = _out_buffers[_out_write_index];

	if(read_buffer.state == Buffer::State::Full) {
		_mcu_block_index += pConvert_Function(
								read_buffer.data, (uint8_t*)jpeg::decoded_buffer_address,
								_mcu_block_index, read_buffer.datasize,
								&converted_data_count
							);
	
		read_buffer.state = Buffer::State::Empty;
		read_buffer.datasize = 0;
	
		_out_read_index = (_out_read_index + 1) % jpeg::out_buffers_nb;
	}
	else if(_out_paused && write_buffer.state == Buffer::State::Empty && read_buffer.state == Buffer::State::Empty) {
		_out_paused = false;
		HAL_JPEG_Resume(hjpeg, JPEG_PAUSE_RESUME_OUTPUT);
	}

	return _mcu_block_index == _mcu_number;
}

}	// namespace leka
