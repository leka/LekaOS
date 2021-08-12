// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_AUDIO_H_
#define _LEKA_OS_LIB_AUDIO_H_

#include <cmath>
#include <memory>

#include "events/EventQueue.h"
#include "platform/mbed_wait_api.h"
#include "rtos/Thread.h"

#include "CoreDAC.h"
#include "CoreDACTimer.h"
#include "LKCoreFatFs.h"
#include "LKCoreSTM32HalBase.h"
#include "WavFile.h"
#include "WavReader.h"

namespace leka {

class CoreAudio
{
  private:
	enum FileReadingState
	{
		Reading,
		LastBuffer,
		Finished
	};

	static constexpr uint16_t _sectorSize_Bytes		 = 512;	  // SD card sectors are 512B in size
	static constexpr uint16_t _sampleSize_Bytes		 = 2;	  // Supported wav files have a 16 bit sampling
	static constexpr uint16_t _sectorSize_Samples	 = _sectorSize_Bytes / _sampleSize_Bytes;
	static constexpr uint16_t _outBufferSize_Sectors = 2;	// This should be an even number to split easily in 2
	static constexpr uint16_t _outBufferSize_Samples = _outBufferSize_Sectors * _sectorSize_Samples;

  public:
	CoreAudio(LKCoreSTM32HalBase &hal, CoreDAC &dac, CoreDACTimer &timer, rtos::Thread &thread,
			  events::EventQueue &eventQueue);

	void playFile(FIL *file);
	void pause() const;
	void resume() const;
	void stop();

	void setVolume(float volume);

	[[nodiscard]] auto isPlaying() const -> bool;

  private:
	LKCoreSTM32HalBase &_hal;
	CoreDAC &_coreDac;
	CoreDACTimer &_coreTimer;

	rtos::Thread &_thread;
	events::EventQueue &_eventQueue;

	static std::array<uint16_t, _outBufferSize_Samples> _outBuffer;
	uint16_t *_outBuffStartPtr	= nullptr;
	uint16_t *_outBuffMiddlePtr = nullptr;

	std::unique_ptr<WavFile> _wavFile = nullptr;

	float _volume						   = 100;
	bool _playing						   = false;
	FileReadingState _fileReadingStateFlag = FileReadingState::Finished;

	void _initialize(uint32_t samplingFreq);
	void _align12bR(lstd::span<uint16_t> samplesBuffer) const;
	void _scaleToVolume(lstd::span<uint16_t> samplesBuffer) const;
	void _onHalfBuffRead();
	void _onFullBuffRead();
	void _handleCallback(uint16_t *buffer);
	void _handleNextSector(uint16_t *buffer);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_AUDIO_H_
