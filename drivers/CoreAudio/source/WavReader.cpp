#include "WavReader.h"

namespace leka {
auto WavReader::loadSector(WavFile *wavfile, uint16_t *buffer, uint16_t sectorSize) -> bool
{
	// TODO(samhadjes) handle less samples read (e.g. writing 0 on rest of buffer)
	bool eof = false;
	if (_readSector(wavfile, buffer, sectorSize) != sectorSize) {
		eof = true;
	}
	_convertSectorData(buffer, sectorSize);

	return eof;
}

// PRIVATE METHODS

// Read next sector to specified buffer
auto WavReader::_readSector(WavFile *wavfile, uint16_t *buffer, uint16_t sectorSize) -> int
{
	int bytesRead = 0;
	if (f_read(wavfile->file(), buffer, sectorSize, (UINT *)&bytesRead) != FR_OK) {
		// TODO(samhadjes) : check if there should be error control here
	}
	return bytesRead;
}

void WavReader::_convertSectorData(uint16_t *buffer, uint16_t sectorSize)
{
	int16_t tmp = 0;
	for (uint16_t i = 0; i < sectorSize; ++i) {
		tmp = 0x0 | buffer[i];	 // cast bits from uint to int
		tmp += 0x8000;			 // rescale to uint scale
		buffer[i] = 0x0 | tmp;
	}
}

}	// namespace leka