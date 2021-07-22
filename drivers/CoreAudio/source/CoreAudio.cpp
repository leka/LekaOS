// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAudio.h"

//

namespace leka {

uint16_t CoreAudio::_waveBuffer[512];
// CoreAudio *CoreAudio::_self					  = nullptr;
events::EventQueue *CoreAudio::_eventQueue	  = nullptr;
CoreAudio::EndOfFileState CoreAudio::_eofFlag = NotFinished;
WavFile *CoreAudio::_wavFile				  = nullptr;

CoreAudio::CoreAudio(LKCoreSTM32HalBase &hal, interface::Dac &dac, interface::DacTimer &timer, rtos::Thread &thread,
					 events::EventQueue &eventQueue)
	: playing(false),
	  _hal(hal),
	  _coreDac(dac),
	  _coreTimer(timer),
	  _thread(thread),
	  //_eventQueue(eventQueue),
	  _volume(100)
//_eofFlag(EndOfFileState::NotFinished)
{
	//_self		= this;
	_eventQueue = &eventQueue;
	_thread.start({_eventQueue, &events::EventQueue::dispatch_forever});
	// printf("CoreAudio constructed\n");
}

void CoreAudio::playFile(FIL *file)
{
	playing					= true;
	_eofFlag				= NotFinished;
	_wavFile				= new WavFile(file);
	uint16_t *_waveBuffer_2 = _waveBuffer + 256;

	// fillBufferWithSinWave(_waveBuffer, 512, 220 , 44100, 0xFFF, 0);
	// fillBufferWithSquare(_waveBuffer, 512, uint16_t maxValue, uint16_t minValue)

	_initialize(CoreAudio::_wavFile->header().SamplingRate);
	printf("This :  : %p\n", this);
	WavReader::loadSector(CoreAudio::_wavFile, _waveBuffer, 512);
	_scaleToVolume(_waveBuffer, 256);
	_align12bR(_waveBuffer, 256);
	// bool eof = false;
	bool eof = WavReader::loadSector(CoreAudio::_wavFile, _waveBuffer_2, 512);
	_scaleToVolume(_waveBuffer_2, 256);
	_align12bR(_waveBuffer_2, 256);
	if (eof) {
		_eofFlag = LastBuffer;
	}
	// printf("about to start \n");
	_coreTimer.start();
	_coreDac.start(_waveBuffer, 512);
	// printf("has started \n");

	// while (!eof) {
	// 	TODO update method
	// 	if (_coreDac.dmaFlag() == CoreDAC::Half_cpt) {
	// 		eof = WavReader::loadSector(&wavFile, _waveBuffer, 512);
	// 		_scaleToVolume(_waveBuffer, 256);
	// 		_align12bR(_waveBuffer, 256);
	// 		_coreDac.dmaFlag() = CoreDAC::None;
	// 		// printf("half DMA\n");
	// 	}

	// 	if (_coreDac.dmaFlag() == CoreDAC::Cpt) {
	// 		if (!eof) {
	// 			eof = WavReader::loadSector(&wavFile, _waveBuffer_2, 512);
	// 			_scaleToVolume(_waveBuffer_2, 256);
	// 			_align12bR(_waveBuffer_2, 256);
	// 		}
	// 		_coreDac.dmaFlag() = CoreDAC::None;
	// 		// printf("cpt DMA\n");
	// 	}
	// }

	// stop();
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
	printf("Stopping all\n");
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
		//*buffer = static_cast<double>(*buffer) /6.F;
		buffer[i] += 0x7FFF * (1.F - _volume / 100.F);
		// buffer[i] = buffer[i] / 2;
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
	_eventQueue->call(this, &CoreAudio::_handleCallback, _waveBuffer);
}

void CoreAudio::_cptCallback()
{
	_eventQueue->call(this, &CoreAudio::_handleCallback, _waveBuffer + 256);
}

void CoreAudio::_handleCallback(uint16_t *buffer)
{
	buffer == _waveBuffer ? printf("Callback half complete handled\n") : printf("Callback complete handled\n");

	if (_eofFlag == NotFinished) {
		printf("case NotFinished\n");
		bool eof = WavReader::loadSector(_wavFile, buffer, 512);
		_scaleToVolume(buffer, 256);
		_align12bR(buffer, 256);
		if (eof) {
			_eofFlag = LastBuffer;
		}
	} else if (_eofFlag == LastBuffer) {
		printf("case LastBuffer\n");
		_eofFlag = Finished;

	} else if (_eofFlag == Finished) {
		printf("case Finished\n");
		stop();
	}
}
}	// namespace leka
