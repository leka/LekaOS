// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_WAVFILE_H_
#define _LEKA_OS_LIB_WAVFILE_H_

#include <cstdint>
#include "LKCoreFatFs.h"
#include "cstdio"
#include "mbed.h"

#define FILE_TYPE_ID 0x52494646 /* correspond to the letters 'RIFF' */
#define FILE_FORMAT	 0x57415645 /* correspond to the letters 'WAVE' */
#define FORMAT_ID	 0x666D7420 /* correspond to the letters 'fmt ' */
#define DATA_ID		 0x64617461 /* correspond to the letters 'data' */

#define HEADER_SIZE 44

namespace leka
{

class WavFile 
{
  private:
    using Endianness = enum { LittleEndian, BigEndian };

  public:
    using WavHeader = struct wavHeader {
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

    WavFile(FIL* file);
    auto file() -> FIL* { return _file;}
    auto header() -> WavHeader& { return _header;};

  private:
    FIL *_file;
    WavHeader _header{};

    void _readHeader();
    auto _readHeaderDataUnit(uint8_t *buffer, uint8_t idx, uint8_t numOfBytes, Endianness bytesFormat) -> uint32_t;
};


} // namespace leka

#endif