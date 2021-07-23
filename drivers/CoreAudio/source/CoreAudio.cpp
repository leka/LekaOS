// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAudio.h"

//

namespace leka {

uint16_t CoreAudio::_waveBuffer[768];

CoreAudio::CoreAudio(LKCoreSTM32HalBase &hal, interface::Dac &dac, interface::DacTimer &timer, rtos::Thread &thread,
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

	_coreTimer.start();
	_coreDac.start(_waveBuffer, 512);
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
// PRIVATE

void CoreAudio::_initialize(float frequency)
{
	static auto *self = this;
	_coreDac.setCptCallbackPtr([](DAC_HandleTypeDef *hdac) { self->_cptCallback(); });
	_coreDac.setHalfCptCallbackPtr([](DAC_HandleTypeDef *hdac) { self->_halfCptCallback(); });

	_coreTimer.initialize(frequency);

	_coreDac.initialize();
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

// Fill buffer with sin wave at given frequency and sampling rate
// Values can be limited between maxValue and minValue
void CoreAudio::fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t frequency, uint32_t samplingRate,
									  uint16_t maxValue, uint16_t minValue)
{
	uint32_t samplesPerPeriod = (samplingRate / frequency);

	for (uint32_t i = 0; i < bufferSize; ++i) {
		float tmp = 0.5 * sin(i * 2.0 * M_PI / samplesPerPeriod) + 0.5;
		tmp *= maxValue - minValue;
		buffer[i] = tmp + minValue;
	}
}

void CoreAudio::fillBufferWithSquare(uint16_t *buffer, uint32_t bufferSize, uint16_t maxValue, uint16_t minValue)
{
	for (uint32_t i = 0; i < bufferSize / 2; ++i) {
		buffer[i] = maxValue;
	}
	for (uint32_t i = bufferSize / 2; i < bufferSize; ++i) {
		buffer[i] = minValue;
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
