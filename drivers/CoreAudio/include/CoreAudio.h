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
	enum EndOfFileState
	{
		NotFinished,
		LastBuffer,
		Finished
	};

	static constexpr uint16_t sectorSize_Bytes		= 512;
	static constexpr uint16_t sampleSize_Bytes		= 2;
	static constexpr uint16_t sectorSize_Samples	= sectorSize_Bytes / sampleSize_Bytes;
	static constexpr uint16_t outBufferSize_Sectors = 2;
	static constexpr uint16_t outBufferSize_Samples = 512;

  public:
	CoreAudio(LKCoreSTM32HalBase &hal, CoreDAC &dac, CoreDACTimer &timer, rtos::Thread &thread,
			  events::EventQueue &eventQueue);
	void playFile(FIL *file);
	void pause() const;
	void resume() const;
	void stop();
	void setVolume(float volume) { _volume = volume; };
	[[nodiscard]] auto isPlaying() const -> bool { return _playing; };

  private:
	static std::array<uint16_t, outBufferSize_Samples> _waveBuffer;
	LKCoreSTM32HalBase &_hal;
	CoreDAC &_coreDac;
	CoreDACTimer &_coreTimer;

	rtos::Thread &_thread;
	events::EventQueue &_eventQueue;

	std::unique_ptr<WavFile> _wavFile = nullptr;

	float _volume			= 100;
	EndOfFileState _eofFlag = EndOfFileState::NotFinished;
	bool _playing			= false;

	void _initialize(uint32_t frequency);
	void _align12bR(uint16_t *buffer, uint16_t nbSamples) const;
	void _scaleToVolume(uint16_t *buffer, uint16_t nbSamples) const;
	void _onHalfBuffRead();
	void _onFullBuffRead();
	void _handleCallback(uint16_t *buffer);
	void _handleNextSector(uint16_t *buffer);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_AUDIO_H_
