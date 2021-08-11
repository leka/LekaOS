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

void CoreAudio::playFile(FIL *file)
{
	playing					= true;
	_eofFlag				= NotFinished;
	_wavFile				= new WavFile(file);
	uint16_t *_waveBuffer_2 = &_waveBuffer[256];

	_initialize(_wavFile->header().SamplingRate);

	_handleNextSector(_waveBuffer);
	_handleNextSector(_waveBuffer_2);

	auto outSpan = lstd::span {_waveBuffer, 512};	// TODO() : check this

	_coreTimer.start();
	_coreDac.start(outSpan);
}

void CoreAudio::pause()
{
	_coreTimer.stop();
}

void CoreAudio::resume()
{
	_coreTimer.start();
}

void CoreAudio::stop()
{
	_coreTimer.stop();
	_coreDac.stop();
	playing = false;
}

void CoreAudio::_initialize(float frequency)
{
	// setup DAC callbacks
	static auto *self = this;
	auto halfBuffCb =
		static_cast<pDAC_CallbackTypeDef>([]([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->_halfCptCallback(); });
	auto fullBuffCb =
		static_cast<pDAC_CallbackTypeDef>([]([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->_cptCallback(); });

	// initialize components
	_coreTimer.initialize(frequency);
	_coreDac.initialize(_coreTimer, halfBuffCb, fullBuffCb);
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

void CoreAudio::_halfCptCallback()
{
	_eventQueue.call(this, &CoreAudio::_handleCallback, _waveBuffer);
}

void CoreAudio::_cptCallback()
{
	_eventQueue.call(this, &CoreAudio::_handleCallback, &(_waveBuffer[256]));
}

void CoreAudio::_handleCallback(uint16_t *buffer)
{
	if (_eofFlag == NotFinished) {
		_handleNextSector(buffer);
	} else if (_eofFlag == LastBuffer) {
		_eofFlag = Finished;

	} else if (_eofFlag == Finished) {
		stop();
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
