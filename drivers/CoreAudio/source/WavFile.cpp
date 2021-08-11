// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WavFile.h"

namespace leka {

WavFile::WavFile(FIL *file) : _file(file)
{
	_readHeader();
}

void WavFile::_readHeader()
{
	uint8_t bytesRead	  = 0;
	uint8_t indexInBuffer = 0;

	uint8_t buffer[HEADER_SIZE] = {0};

	f_lseek(_file, 0);	 // move the file pointer to start of file

	if (f_read(_file, buffer, HEADER_SIZE, (UINT *)&bytesRead) != FR_OK) {
		// TODO(samhadjes) : handle error
	}

	// f_lseek(_file, 0);	// reset position
	// TODO(samhadjes) : check if starting after header is problematic

	_header.FileTypeBlockID = _readHeaderDataUnit(buffer, indexInBuffer, 4, BigEndian);
	if (_header.FileTypeBlockID != FILE_TYPE_ID) {
		// printf("File is not RIFF\n");

		// return 1;
		// TODO(samhadjes) : handle error
	}
	indexInBuffer += 4;

	_header.FileSize = _readHeaderDataUnit(buffer, indexInBuffer, 4, LittleEndian);	  // Total size - 8 bytes
	indexInBuffer += 4;

	_header.FileFormatID = _readHeaderDataUnit(buffer, indexInBuffer, 4, BigEndian);
	if (_header.FileFormatID != FILE_FORMAT) {
		// printf("File is not WAVE\n");
		// return 1;
		// TODO(samhadjes) : handle error
	}
	indexInBuffer += 4;

	_header.FormatBlockID = _readHeaderDataUnit(buffer, indexInBuffer, 4, BigEndian);
	if (_header.FormatBlockID != FORMAT_ID) {
		// printf("Format definition block not found\n");
		// return 1;
		// TODO(samhadjes) : handle error
	}
	indexInBuffer += 4;

	_header.FormatBlockSize = _readHeaderDataUnit(buffer, indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;

	_header.AudioFormat = _readHeaderDataUnit(buffer, indexInBuffer, 2, LittleEndian);
	if (_header.AudioFormat != 1) {
		// printf("Only integer PCM format is supported\n");
		// return 1;
		// TODO(samhadjes) : handle error
	}
	indexInBuffer += 2;

	_header.NumChannels = _readHeaderDataUnit(buffer, indexInBuffer, 2, LittleEndian);
	if (_header.NumChannels != 1) {
		// printf("Only mono (1 channel) sounds are supported\n");
		// return 1;
		// TODO(samhadjes) : handle error
	}
	indexInBuffer += 2;

	_header.SamplingRate = _readHeaderDataUnit(buffer, indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;

	_header.BytePerSec = _readHeaderDataUnit(buffer, indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;

	_header.BytePerSampleBlock = _readHeaderDataUnit(buffer, indexInBuffer, 2, LittleEndian);
	indexInBuffer += 2;

	_header.BitsPerSample = _readHeaderDataUnit(buffer, indexInBuffer, 2, LittleEndian);
	if (_header.BitsPerSample != 16) {
		// printf("Only 16 bits sampling is supported\n");
		// return 1;
	}
	indexInBuffer += 2;

	_header.DataBlockID = _readHeaderDataUnit(buffer, indexInBuffer, 4, BigEndian);
	if (_header.DataBlockID != DATA_ID) {
		// printf("Data block not found\n");
		// return 1;
	}
	indexInBuffer += 4;

	_header.DataSize = _readHeaderDataUnit(buffer, indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;
}

// read bytes from buffer and reorganize them in the right order
// max uint32_t
auto WavFile::_readHeaderDataUnit(uint8_t *buffer, uint8_t idx, uint8_t numOfBytes, Endianness bytesFormat) -> uint32_t
{
	uint32_t index = 0;
	uint32_t temp  = 0;

	for (index = 0; index < numOfBytes; index++) {
		temp |= buffer[idx + index] << (index * 8);
	}

	if (bytesFormat == BigEndian) {
		temp = _reverseEndianness(temp, numOfBytes);
	}

	return temp;
}

auto WavFile::_reverseEndianness(uint32_t val, uint8_t numBytes) -> uint32_t
{
	uint32_t index = 0;
	uint32_t temp  = 0;
	auto *buffer   = (uint8_t *)&val;
	for (index = 0; index < numBytes; index++) {
		temp |= buffer[index] << ((numBytes - 1 - index) * 8);
	}
	return temp;
}

}	// namespace leka
