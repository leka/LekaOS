// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAudio.h"

namespace leka {

uint16_t CoreAudio::_waveBuffer[512];

CoreAudio::CoreAudio(LKCoreSTM32HalBase &hal, CoreDAC &dac, CoreDACTimer &timer, rtos::Thread &thread,
					 events::EventQueue &eventQueue)
	: playing(false),
	  _hal(hal),
	  _coreDac(dac),
	  _coreTimer(timer),
	  _thread(thread),
	  _eventQueue(eventQueue),
	  _volume(100),
	  _eofFlag(EndOfFileState::NotFinished)
{
	_thread.start({&_eventQueue, &events::EventQueue::dispatch_forever});
}

void CoreAudio::_align12bR(uint16_t *buffer, uint16_t length)
{
	for (int i = 0; i < length; ++i) {
		buffer[i] = buffer[i] >> 4;
	}
}

void CoreAudio::_scaleToVolume(uint16_t *buffer, uint16_t length)
{
	for (int i = 0; i < length; ++i) {
		buffer[i] = static_cast<double>(buffer[i]) * (_volume / 100.F);
		buffer[i] += 0x7FFF * (1.F - _volume / 100.F);
	}
}

void CoreAudio::_handleNextSector(uint16_t *buffer)
{
	bool eof = WavReader::loadSector(_wavFile, buffer, 512);
	_scaleToVolume(buffer, 256);
	_align12bR(buffer, 256);
	if (eof) {
		_eofFlag = LastBuffer;
	}
}

}	// namespace leka
