// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_AUDIO_H_
#define _LEKA_OS_LIB_AUDIO_H_

#include <cmath>

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

  public:
	CoreAudio(LKCoreSTM32HalBase &hal, CoreDAC &dac, CoreDACTimer &timer, rtos::Thread &thread,
			  events::EventQueue &eventQueue);
	void playFile(FIL *file);
	void pause();
	void resume();
	void stop();
	void setVolume(float volume) { _volume = volume; };

	void fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t frequency, uint32_t samplingRate = 44100,
							   uint16_t maxValue = 0xFFFF, uint16_t minValue = 0x0);
	void fillBufferWithSquare(uint16_t *buffer, uint32_t bufferSize, uint16_t maxValue, uint16_t minValue);

	bool playing;

  private:
	static uint16_t _waveBuffer[512];
	LKCoreSTM32HalBase &_hal;
	CoreDAC &_coreDac;
	CoreDACTimer &_coreTimer;

	WavFile *_wavFile;

	rtos::Thread &_thread;
	events::EventQueue &_eventQueue;

	float _volume;
	EndOfFileState _eofFlag;

	void _initialize(float frequency);
	void _align12bR(uint16_t *buffer, uint16_t length);
	void _scaleToVolume(uint16_t *buffer, uint16_t length);
	void _halfCptCallback();
	void _cptCallback();
	void _handleCallback(uint16_t *buffer);
	void _handleNextSector(uint16_t *buffer);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_AUDIO_H_
