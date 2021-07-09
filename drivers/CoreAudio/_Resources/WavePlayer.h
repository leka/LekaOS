// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_WAVE_PLAYER_H_
#define _LEKA_OS_SPIKE_WAVE_PLAYER_H_

//#include <chrono>
//#include <cstdio>

//#include "PeripheralPins.h"

//#include "AudioUtils.h"
//#include "mbed.h"
//#include <cstdint>
//#include <string>

#include "FileManager.h"
#include "WavePlayerSetup.h"
//#include "stm32f7xx_hal_tim.h"

#define FILE_TYPE_ID 0x52494646 /* correspond to the letters 'RIFF' */
#define FILE_FORMAT	 0x57415645 /* correspond to the letters 'WAVE' */
#define FORMAT_ID	 0x666D7420 /* correspond to the letters 'fmt ' */
#define DATA_ID		 0x64617461 /* correspond to the letters 'data' */

using CurrentBuffer = enum { Buffer_1, Buffer_2 };

using Endianness = enum { LittleEndian, BigEndian };

uint16_t _waveBuffer_1[SAMPLES_IN_BUFF] = {0};
uint16_t *_waveBuffer_2					= _waveBuffer_1 + SAMPLES_IN_HALF_BUFF;

// structs
using WaveHeader = struct wavHeader {
	uint32_t FileTypeBlockID;
	uint32_t FileSize;
	uint32_t FileFormatID;
	uint32_t FormatBlockID;
	uint32_t FormatBlockSize;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SamplingRate;
	uint32_t BytePerSec;
	uint16_t BytePerSampleBlock;
	uint16_t BitsPerSample;
	uint32_t DataBlockID;
	uint32_t DataSize;
};

// Read 512 bytes to specified buffer
auto ReadFileSector(FIL *file, uint8_t *buffer) -> int
{
	int bytesRead = 0;
	if (f_read(file, buffer, 512, (UINT *)&bytesRead) != FR_OK) {
	}

	return bytesRead;
}

// void scaleU16Val(uint16_t &val, uint16_t min, uint16_t max)
// {
// 	val = val * (max - min) / 0xFFFF;
// 	val += min;
// }

// convert one sample data
void convertData(uint16_t *valSrc, uint16_t *valDst)
{
	int16_t b = 0x0 | *valSrc;
	// b		  = b / 4;
	b += 0x8000;   // +0x2000 for great sound
	*valDst = 0x0 | b;
	*valDst = *valDst >> 4;	  // TODO CHANGE
}

// parse whole buffer to convert data before the ISR handles it
void convertBufferData(uint16_t *src, uint16_t *dst, uint32_t len)
{
	for (uint32_t i = 0; i < len; ++i) {
		convertData(&(src[i]), &(dst[i]));
	}
}

// read bytes from buffer and reorganize them in the right order
// max uint32_t
auto ReadUnit(uint8_t *buffer, uint8_t idx, uint8_t numOfBytes, Endianness bytesFormat) -> uint32_t
{
	uint32_t index = 0;
	uint32_t Temp  = 0;

	for (index = 0; index < numOfBytes; index++) {
		Temp |= buffer[idx + index] << (index * 8);
	}

	if (bytesFormat == BigEndian) {
		Temp = __REV(Temp);
	}
	return Temp;
}

auto samplingRateAllowed(uint32_t sRate) -> bool
{
	if (sRate == 11025) return true;
	if (sRate == 22050) return true;
	if (sRate == 44100) return true;
	if (sRate == 48000) return true;
	if (sRate == 96000) return true;
	return false;
}

auto ReadWaveHeader(uint8_t *waveBuffer, WaveHeader &header) -> int
{
	uint8_t indexInBuffer = 0;

	header.FileTypeBlockID = ReadUnit(waveBuffer, indexInBuffer, 4, BigEndian);
	if (header.FileTypeBlockID != FILE_TYPE_ID) {
		printf("File is not RIFF\n");
		return 1;
	}
	indexInBuffer += 4;

	header.FileSize = ReadUnit(waveBuffer, indexInBuffer, 4, LittleEndian);	  // Total size - 8 bytes
	indexInBuffer += 4;

	header.FileFormatID = ReadUnit(waveBuffer, indexInBuffer, 4, BigEndian);
	if (header.FileFormatID != FILE_FORMAT) {
		printf("File is not WAVE\n");
		return 1;
	}
	indexInBuffer += 4;

	header.FormatBlockID = ReadUnit(waveBuffer, indexInBuffer, 4, BigEndian);
	if (header.FormatBlockID != FORMAT_ID) {
		printf("Format definition block not found\n");
		return 1;
	}
	indexInBuffer += 4;

	header.FormatBlockSize = ReadUnit(waveBuffer, indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;

	header.AudioFormat = ReadUnit(waveBuffer, indexInBuffer, 2, LittleEndian);
	if (header.AudioFormat != 1) {
		printf("Only integer PCM format is supported\n");
		return 1;
	}
	indexInBuffer += 2;

	header.NumChannels = ReadUnit(waveBuffer, indexInBuffer, 2, LittleEndian);
	if (header.NumChannels != 1) {
		printf("Only mono (1 channel) sounds are supported\n");
		return 1;
	}
	indexInBuffer += 2;

	header.SamplingRate = ReadUnit(waveBuffer, indexInBuffer, 4, LittleEndian);
	if (!samplingRateAllowed(header.SamplingRate)) {
		printf("%lu Hz sampling rate for this file is not supported\n", header.SamplingRate);
		return 1;
	}

	indexInBuffer += 4;

	header.BytePerSec = ReadUnit(waveBuffer, indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;

	header.BytePerSampleBlock = ReadUnit(waveBuffer, indexInBuffer, 2, LittleEndian);
	indexInBuffer += 2;

	header.BitsPerSample = ReadUnit(waveBuffer, indexInBuffer, 2, LittleEndian);
	if (header.BitsPerSample != 16) {
		printf("Only 16 bits sampling is supported\n");
		return 1;
	}
	indexInBuffer += 2;

	header.DataBlockID = ReadUnit(waveBuffer, indexInBuffer, 4, BigEndian);
	if (header.DataBlockID != DATA_ID) {
		printf("Data block not found\n");
		return 1;
	}
	indexInBuffer += 4;

	header.DataSize = ReadUnit(waveBuffer, indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;

	return 0;
}

auto PlayWavFile(std::string filename) -> int
{
	FIL file;
	wavHeader header;
	uint16_t bytesRead;
	u_int32_t totalBytesRead = 0;

	// Read first sector of file
	if (f_open(&file, filename.c_str(), FA_READ) == FR_OK) {
		if ((bytesRead = ReadFileSector(&file, (uint8_t *)_waveBuffer_1)) < 44)	  // read sector
		{
			printf("File is too short to contain data, only read %d bytes\n", bytesRead);
			return 1;
		}
	} else {
		printf("Could not open file : %s\n", filename.c_str());
		return 1;
	}
	totalBytesRead += bytesRead;
	// parse header
	if (ReadWaveHeader((uint8_t *)_waveBuffer_1, header) != 0) {
		f_close(&file);
		return 1;
	}
	printf("Read header\n");
	printf("Audio format: %u\n", header.AudioFormat);
	printf("bits per sample: %u\n", header.BitsPerSample);
	convertBufferData(_waveBuffer_1, _waveBuffer_1, SAMPLES_IN_HALF_BUFF);

	init_TIM_DAC_DMA();
	start_TIM_DAC_DMA((uint32_t *)_waveBuffer_1);

	bytesRead = ReadFileSector(&file, (uint8_t *)(_waveBuffer_2));	 // load buffer 2;
	convertBufferData(_waveBuffer_2, _waveBuffer_2, SAMPLES_IN_HALF_BUFF);
	totalBytesRead += bytesRead;

	while (bytesRead == 512) {
		if (_flag == DMA1_half_cpt) {
			bytesRead = ReadFileSector(&file, (uint8_t *)_waveBuffer_1);
			convertBufferData(_waveBuffer_1, _waveBuffer_1, SAMPLES_IN_HALF_BUFF);	 // convert data
			totalBytesRead += bytesRead;
			_flag = None;
		}

		if (_flag == DMA1_cpt) {
			if (bytesRead == 512) {
				bytesRead = ReadFileSector(&file, (uint8_t *)(_waveBuffer_2));
				convertBufferData(_waveBuffer_2, _waveBuffer_2, SAMPLES_IN_HALF_BUFF);	 // convert data
				totalBytesRead += bytesRead;
			}
			_flag = None;
		}
	}

	f_close(&file);

	HAL_TIM_Base_Stop(&htim6);
	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);

	HAL_TIM_Base_DeInit(&htim6);
	HAL_DAC_DeInit(&hdac);
	HAL_DMA_DeInit(&hdma_dac1);

	return 0;
}

#endif	 // _LEKA_OS_SPIKE_WAVE_PLAYER_H_