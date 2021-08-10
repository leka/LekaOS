// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_VIBRATION_H_
#define _LEKA_OS_LIB_VIBRATION_H_

#include <cmath>
#include <cstdint>

#include "events/EventQueue.h"
#include "rtos/ThisThread.h"
#include "rtos/Thread.h"

#include "CoreDAC.h"
#include "VibrationTemplate.h"

namespace leka {

class CoreVibration
{
  public:
	CoreVibration(LKCoreSTM32HalBase &hal, CoreDAC &dac, CoreDACTimer &timer, rtos::Thread &thread,
				  events::EventQueue &eventQueue);

	void initialize(uint32_t samplingRate);
	void deInit();

	void play(VibrationTemplate &vib);
	void stop();
	void playPeriodically(VibrationTemplate &vib, fseconds waitTime, uint16_t nbRep);

	[[nodiscard]] auto isPlaying() const -> bool;
	[[nodiscard]] auto isPlayingPeriodically() const -> bool;

  private:
	LKCoreSTM32HalBase &_hal;
	CoreDAC &_coreDac;
	CoreDACTimer &_coreTimer;

	rtos::Thread &_thread;
	events::EventQueue &_eventQueue;

	std::vector<uint16_t> _vibBuffer;
	uint16_t *_vibBufferPtr_1 = nullptr;
	uint16_t *_vibBufferPtr_2 = nullptr;

	VibrationTemplate *_currentVib = nullptr;

	std::vector<float> _sinBuffer;
	std::vector<float> _tmpBuffer;
	uint32_t _samplingRate	   = 0;
	uint32_t _samplesPerPeriod = 0;

	bool _isPlaying				= false;
	bool _isPlayingPeriodically = false;

	void start();

	void createSinWavePeriod(float *sinBuffer, const VibrationTemplate &vib) const;
	void rescaleVibration();
	void fillHalfBuffer(uint16_t *buffer, uint32_t nbSamples);

	void halfBufferCallback();
	void cptBufferCallback();
	void handleCallback(uint16_t *buffer);

	void endPeriodicVib(int eventID);
	void playPtr(VibrationTemplate *vib);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_H_