// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAudio.h"

namespace leka {

std::array<uint16_t, CoreAudio::_outBufferSize_Samples> CoreAudio::_outBuffer;

CoreAudio::CoreAudio(LKCoreSTM32HalBase &hal, DACDriver &dac, DACTimer &timer, rtos::Thread &thread,
					 events::EventQueue &eventQueue)
	: _hal(hal), _dac(dac), _dacTimer(timer), _thread(thread), _eventQueue(eventQueue)
{
	_thread.start({&_eventQueue, &events::EventQueue::dispatch_forever});
	_outBuffStartPtr  = _outBuffer.data();
	_outBuffMiddlePtr = _outBuffer.data() + (_outBufferSize_Samples / 2);
}

void CoreAudio::playFile(FIL *file)
{
	_playing			  = true;
	_fileReadingStateFlag = Reading;
	_wavFile			  = std::make_unique<WavFile>(file);

	_initialize(_wavFile->getHeader().SamplingRate);

	_handleNextSector(_outBuffStartPtr);
	_handleNextSector(_outBuffMiddlePtr);

	_dac.start(lstd::span {_outBuffStartPtr, _outBufferSize_Samples});
	_dacTimer.start();
}

void CoreAudio::pause() const
{
	_dacTimer.stop();
}

void CoreAudio::resume() const
{
	_dacTimer.start();
}

void CoreAudio::stop()
{
	_dacTimer.stop();
	_dac.stop();
	_playing = false;
}

void CoreAudio::setVolume(float volume)
{
	_volume = volume;
}

auto CoreAudio::isPlaying() const -> bool
{
	return _playing;
}

void CoreAudio::_initialize(uint32_t samplingFreq)
{
	static auto *self = this;
	auto halfBuffCb =
		static_cast<pDAC_CallbackTypeDef>([]([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->_onHalfBuffRead(); });
	auto fullBuffCb =
		static_cast<pDAC_CallbackTypeDef>([]([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->_onFullBuffRead(); });

	_dacTimer.initialize(samplingFreq);
	_dac.initialize(_dacTimer, halfBuffCb, fullBuffCb);
}

void CoreAudio::_align12bR(lstd::span<uint16_t> samplesBuffer) const
{
	for (auto &sample: samplesBuffer) {
		sample = sample >> 4;
	}
}

void CoreAudio::_scaleToVolume(lstd::span<uint16_t> samplesBuffer) const
{
	constexpr uint16_t offset = 0x7FFF;

	for (auto &sample: samplesBuffer) {
		sample = static_cast<uint16_t>((static_cast<float>(sample) * _volume) / 100);
		sample += static_cast<uint16_t>(offset * (1.F - _volume / 100.F));
	}
}

void CoreAudio::_onHalfBuffRead()
{
	_eventQueue.call(this, &CoreAudio::_handleCallback, _outBuffStartPtr);
}

void CoreAudio::_onFullBuffRead()
{
	_eventQueue.call(this, &CoreAudio::_handleCallback, _outBuffMiddlePtr);
}

void CoreAudio::_handleCallback(uint16_t *buffer)
{
	if (_fileReadingStateFlag == Reading) {
		_handleNextSector(buffer);

	} else if (_fileReadingStateFlag == LastBuffer) {
		_fileReadingStateFlag = Finished;

	} else if (_fileReadingStateFlag == Finished) {
		stop();
	}
}

void CoreAudio::_handleNextSector(uint16_t *buffer)
{
	bool eof		 = WavReader::loadSector(*_wavFile, buffer, _sectorSize_Bytes);
	auto samplesBuff = lstd::span {buffer, _sectorSize_Samples};
	_scaleToVolume(samplesBuff);
	_align12bR(samplesBuff);   // DAC configured to work with the 12 rightmost bits of uint16_t
	if (eof) {
		_fileReadingStateFlag = LastBuffer;
	}
}

}	// namespace leka
