// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WavFile.h"

#include "LogKit.h"

namespace leka {

WavFile::WavFile(FIL *file) : _file(file)
{
	_readHeader();
}

void WavFile::_readHeader()
{
	uint8_t bytesRead	  = 0;
	uint8_t indexInBuffer = 0;

	std::array<uint8_t, sizeof(WavHeader)> buffer = {};

	f_lseek(_file, 0);	 // move the file pointer to start of file

	if (f_read(_file, buffer.data(), sizeof(WavHeader), (UINT *)&bytesRead) != FR_OK) {
		log_error("Error while reading wav file header");
	}

	_header.FileTypeBlockID = _readHeaderDataUnit(buffer.data(), indexInBuffer, 4, BigEndian);
	if (_header.FileTypeBlockID != blockID_fileType) {
		log_error("File is not RIFF");
	}
	indexInBuffer += 4;

	_header.FileSize =
		_readHeaderDataUnit(buffer.data(), indexInBuffer, 4, LittleEndian);	  // Nb bytes in file -8 bytes
	indexInBuffer += 4;

	_header.FileFormatID = _readHeaderDataUnit(buffer.data(), indexInBuffer, 4, BigEndian);
	if (_header.FileFormatID != blockID_fileFormat) {
		log_error("File is not WAVE");
	}
	indexInBuffer += 4;

	_header.FormatBlockID = _readHeaderDataUnit(buffer.data(), indexInBuffer, 4, BigEndian);
	if (_header.FormatBlockID != blockID_format) {
		log_error("Format definition block not found");
	}
	indexInBuffer += 4;

	_header.FormatBlockSize =
		_readHeaderDataUnit(buffer.data(), indexInBuffer, 4, LittleEndian);	  // Nb bytes in format block -16 bytes
	indexInBuffer += 4;

	_header.AudioFormat = _readHeaderDataUnit(buffer.data(), indexInBuffer, 2, LittleEndian);
	if (_header.AudioFormat != 1) {
		log_error("Only integer PCM format is supported");
	}
	indexInBuffer += 2;

	_header.NumChannels = _readHeaderDataUnit(buffer.data(), indexInBuffer, 2, LittleEndian);
	if (_header.NumChannels != 1) {
		log_error("Only mono (1 channel) sounds are supported");
	}
	indexInBuffer += 2;

	_header.SamplingRate = _readHeaderDataUnit(buffer.data(), indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;

	_header.BytePerSec = _readHeaderDataUnit(buffer.data(), indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;

	_header.BytePerSampleBlock = _readHeaderDataUnit(buffer.data(), indexInBuffer, 2, LittleEndian);
	indexInBuffer += 2;

	_header.BitsPerSample = _readHeaderDataUnit(buffer.data(), indexInBuffer, 2, LittleEndian);
	if (_header.BitsPerSample != 16) {
		log_error("Only 16 bits sampling is supported");
	}
	indexInBuffer += 2;

	_header.DataBlockID = _readHeaderDataUnit(buffer.data(), indexInBuffer, 4, BigEndian);
	if (_header.DataBlockID != blockID_data) {
		log_error("Data block not found");
	}
	indexInBuffer += 4;

	_header.DataSize = _readHeaderDataUnit(buffer.data(), indexInBuffer, 4, LittleEndian);
	indexInBuffer += 4;
}

// max uint32_t
auto WavFile::_readHeaderDataUnit(const uint8_t *buffer, uint8_t idx, uint8_t numOfBytes, Endianness bytesFormat)
	-> uint32_t
{
	if (numOfBytes > 4) {
		log_error("Can't reverse more than 4 bytes");
	}
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
