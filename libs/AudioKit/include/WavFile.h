// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/platform/File.h"

namespace leka {

class WavFile
{
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

	explicit WavFile(interface::File &file) : _file(file) {}

	auto open(std::filesystem::path filename) -> bool;

	[[nodiscard]] auto getHeader() const -> const WavHeader &;

	auto read(std::span<int16_t> buffer) -> bool;
	[[nodiscard]] auto isEndOfFile() const -> bool;

  private:
	void readHeader();

	interface::File &_file;

	const std::filesystem::path directory {"/fs/home/wav"};
	const std::string extension {".wav"};

	WavHeader _header {};

	bool _is_eof {false};
};

}	// namespace leka
