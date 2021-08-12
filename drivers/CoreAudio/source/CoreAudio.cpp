// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAudio.h"

namespace leka {

std::array<uint16_t, CoreAudio::outBufferSize_Samples> CoreAudio::_waveBuffer;

CoreAudio::CoreAudio(LKCoreSTM32HalBase &hal, CoreDAC &dac, CoreDACTimer &timer, rtos::Thread &thread,
					 events::EventQueue &eventQueue)
	: _hal(hal), _coreDac(dac), _coreTimer(timer), _thread(thread), _eventQueue(eventQueue)
{
	_thread.start({&_eventQueue, &events::EventQueue::dispatch_forever});
}

void CoreAudio::playFile(FIL *file)
{
	_playing				= true;
	_eofFlag				= NotFinished;
	_wavFile				= make_unique<WavFile>(file);
	uint16_t *_waveBuffer_2 = _waveBuffer.data() + (outBufferSize_Samples / 2);

	_initialize(_wavFile->getHeader().SamplingRate);

	_handleNextSector(_waveBuffer.data());
	_handleNextSector(_waveBuffer_2);

	auto outSpan = lstd::span {_waveBuffer.data(), outBufferSize_Samples};

	_coreTimer.start();
	_coreDac.start(outSpan);   // TODO() : check if span shouldn't be copied instead of given by ref
}

void CoreAudio::pause() const
{
	_coreTimer.stop();
}

void CoreAudio::resume() const
{
	_coreTimer.start();
}

void CoreAudio::stop()
{
	_coreTimer.stop();
	_coreDac.stop();
	_playing = false;
}

void CoreAudio::_initialize(uint32_t frequency)
{
	static auto *self = this;
	auto halfBuffCb =
		static_cast<pDAC_CallbackTypeDef>([]([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->_onHalfBuffRead(); });
	auto fullBuffCb =
		static_cast<pDAC_CallbackTypeDef>([]([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->_onFullBuffRead(); });

	_coreTimer.initialize(frequency);
	_coreDac.initialize(_coreTimer, halfBuffCb, fullBuffCb);
}

void CoreAudio::_align12bR(uint16_t *buffer, uint16_t nbSamples) const
{
	for (int i = 0; i < nbSamples; ++i) {
		buffer[i] = buffer[i] >> 4;
	}
}

void CoreAudio::_scaleToVolume(uint16_t *buffer, uint16_t nbSamples) const
{
	static const uint16_t offset = 0x7FFF;
	for (int i = 0; i < nbSamples; ++i) {
		buffer[i] = static_cast<uint16_t>((static_cast<float>(buffer[i]) * _volume) / 100);
		buffer[i] += static_cast<uint16_t>(offset * (1.F - _volume / 100.F));
	}
}

void CoreAudio::_onHalfBuffRead()
{
	_eventQueue.call(this, &CoreAudio::_handleCallback, _waveBuffer.data());
}

void CoreAudio::_onFullBuffRead()
{
	uint16_t *_waveBuffer_2 = _waveBuffer.data() + (outBufferSize_Samples / 2);
	_eventQueue.call(this, &CoreAudio::_handleCallback, _waveBuffer_2);
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
	bool eof = WavReader::loadSector(*_wavFile, buffer, sectorSize_Bytes);
	_scaleToVolume(buffer, sectorSize_Samples);
	_align12bR(buffer, sectorSize_Samples);	  // DAC configured to work with the 12 rightmost bits of uint16_t
	if (eof) {
		_eofFlag = LastBuffer;
	}
}

}	// namespace leka
