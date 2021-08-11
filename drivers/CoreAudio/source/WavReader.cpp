// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "WavReader.h"

namespace leka {
auto WavReader::loadSector(const WavFile &wavfile, uint16_t *dstBuffer, uint16_t sectorSizeBytes) -> bool
{
	bool eof = false;

	if (UINT bytesRead = _readSector(wavfile, dstBuffer, sectorSizeBytes); bytesRead != sectorSizeBytes) {
		for (auto i = static_cast<uint16_t>(bytesRead); i < sectorSizeBytes; ++i) {
			dstBuffer[i] = 0;
		}

		eof = true;
	}

	_convertSectorData(dstBuffer, sectorSizeBytes / 2);

	return eof;
}

auto WavReader::_readSector(const WavFile &wavfile, uint16_t *dstBuffer, uint16_t sectorSizeBytes) -> UINT
{
	UINT bytesRead = 0;
	if (f_read(wavfile.getFile(), dstBuffer, sectorSizeBytes, &bytesRead) != FR_OK) {
		log_error("Error while reading wav file sector");
	}
	return bytesRead;
}

void WavReader::_convertSectorData(uint16_t *buffer, uint16_t sectorSizeSamples)
{
	int16_t tmp = 0;
	for (uint16_t i = 0; i < sectorSizeSamples; ++i) {
		tmp = 0x0 | buffer[i];	 // cast bits from uint to int
		tmp += 0x8000;			 // rescale to uint scale
		buffer[i] = 0x0 | tmp;
	}
}

}	// namespace leka
