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
	CoreVibration(LKCoreSTM32HalBase &hal, interface::Dac &dac, interface::DacTimer &timer, rtos::Thread &thread,
				  events::EventQueue &eventQueue);

	void initialize(float samplingRate);
	void deInit();

	void play(VibrationTemplate &vib);
	void stop();
	void playPeriodically(VibrationTemplate &vib, fseconds waitTime, uint16_t nbRep);

	[[nodiscard]] auto isPlaying() const -> bool;
	[[nodiscard]] auto isPlayingPeriodically() const -> bool;

	// void playSequence(std::vector<VibrationTemplate> sequence);

  private:
	LKCoreSTM32HalBase &_hal;
	interface::Dac &_coreDac;
	interface::DacTimer &_coreTimer;

	rtos::Thread &_thread;
	events::EventQueue &_eventQueue;

	uint16_t *_vibBuffer_1;
	uint16_t *_vibBuffer_2;

	VibrationTemplate *_currentVib;

	float *_sinBuffer;
	float *_tmpBuffer;
	float _samplingRate;
	uint32_t _samplesPerPeriod;

	bool _isPlaying;
	bool _isPlayingPeriodically;

	void start();

	void createSinWavePeriod(float *sinBuffer, VibrationTemplate &vib) const;
	void rescaleVibration();
	void fillHalfBuffer(uint16_t *buffer, uint32_t nbSamples);

	void halfBufferCallback();
	void cptBufferCallback();
	void handleCallback(u_int16_t *buffer);

	void handlePeriodicCall(VibrationTemplate *vib);
	void endPeriodicVib(int eventID);
	void playPtr(VibrationTemplate *vib);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_H_