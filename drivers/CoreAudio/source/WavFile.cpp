// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WavFile.h"

namespace leka {

WavFile::WavFile(FIL *file) : _file(file)
{
	_readHeader();
}

auto WavFile::getFile() const -> FIL *
{
	return _file;
}

auto WavFile::getHeader() const -> const WavHeader &
{
	return _header;
};

void WavFile::_readHeader()
{
	UINT bytesRead = 0;

	f_lseek(_file, 0);	 // move the file pointer to start of file

	// read header from file directly into header struct memory
	// this works because the data in the file is stored in little endian
	if (f_read(_file, &_header, sizeof(WavHeader), &bytesRead) != FR_OK) {
		log_error("Error while reading wav file header");
	}

	if (_header.FileTypeBlockID != blockID_fileType) {
		log_error("File is not RIFF");
	}

	if (_header.FileFormatID != blockID_fileFormat) {
		log_error("File is not WAVE");
	}

	if (_header.FormatBlockID != blockID_format) {
		log_error("Format definition block not found");
	}

	if (_header.AudioFormat != 1) {
		log_error("Only integer PCM format is supported");
	}

	if (_header.NumChannels != 1) {
		log_error("Only mono (1 channel) sounds are supported");
	}

	if (_header.BitsPerSample != 16) {
		log_error("Only 16 bits sampling is supported");
	}

	if (_header.DataBlockID != blockID_data) {
		log_error("Data block not found");
	}
}

}	// namespace leka
