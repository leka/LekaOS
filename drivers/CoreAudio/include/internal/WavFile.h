// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_WAVFILE_H_
#define _LEKA_OS_LIB_WAVFILE_H_

#include "LKCoreFatFs.h"
#include "LogKit.h"

namespace leka {

class WavFile
{
  private:
	constexpr static uint32_t blockID_fileType	 = 0x46464952;	 // corresponds to the letters 'RIFF' in reverse order
	constexpr static uint32_t blockID_fileFormat = 0x45564157;	 // corresponds to the letters 'WAVE' in reverse order
	constexpr static uint32_t blockID_format	 = 0x20746D66;	 // corresponds to the letters 'fmt ' in reverse order
	constexpr static uint32_t blockID_data		 = 0x61746164;	 // corresponds to the letters 'data' in reverse order

  public:
	using WavHeader = struct wavHeader {
		uint32_t FileTypeBlockID;
		uint32_t FileSize;	 // Nb bytes in file -8 bytes
		uint32_t FileFormatID;
		uint32_t FormatBlockID;
		uint32_t FormatBlockSize;	// Nb bytes in format block -16 bytes
		uint16_t AudioFormat;
		uint16_t NumChannels;
		uint32_t SamplingRate;
		uint32_t BytePerSec;
		uint16_t BytePerSampleBlock;
		uint16_t BitsPerSample;
		uint32_t DataBlockID;
		uint32_t DataSize;
	};

	explicit WavFile(FIL *file);

	[[nodiscard]] auto getFile() const -> FIL *;
	[[nodiscard]] auto getHeader() const -> const WavHeader &;

  private:
	FIL *_file;
	WavHeader _header {};

	void _readHeader();
};

}	// namespace leka

#endif
