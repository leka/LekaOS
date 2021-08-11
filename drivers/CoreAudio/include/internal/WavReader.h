// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_WAVREADER_H_
#define _LEKA_OS_LIB_WAVREADER_H_

#include "LKCoreFatFs.h"
#include "WavFile.h"

namespace leka {

class WavReader
{
  public:
	WavReader() = default;
	static auto loadSector(const WavFile &wavfile, uint16_t *dstBuffer, uint16_t sectorSizeBytes) -> bool;

  private:
	static auto _readSector(const WavFile &wavfile, uint16_t *dstBuffer, uint16_t sectorSizeBytes) -> UINT;
	static void _convertSectorData(uint16_t *buffer, uint16_t sectorSizeSamples);
};

}	// namespace leka

#endif
