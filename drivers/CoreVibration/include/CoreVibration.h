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
	void playPeriodically(VibrationTemplate &vib, fseconds waitTime, uint16_t nbRep);
	void playSequence(std::vector<VibrationTemplate> sequence);
	void play(VibrationTemplate &vib);
	void stop();
	void deInit();

	[[nodiscard]] auto isPlaying() const -> bool;
	[[nodiscard]] auto isPlayingPeriodically() const -> bool;

  private:
	uint16_t *_vibBuffer;
	uint16_t *_vibBuffer_2;

	LKCoreSTM32HalBase &_hal;
	interface::Dac &_coreDac;
	interface::DacTimer &_coreTimer;

	rtos::Thread &_thread;
	events::EventQueue &_eventQueue;

	uint16_t *_sinBuffer;
	int32_t _samplesRemaining;
	float _samplingRate;
	uint32_t _samplesPerPeriod;
	bool _isPlaying;
	bool _isPlayingPeriodically;

	void start();

	void createSinWavePeriod(uint16_t *sinBuffer, VibrationTemplate &vib);
	void rescaleVibration();
	void fillHalfBuffer(uint16_t *buffer, uint32_t numOfSamples);

	void halfBufferCallback();
	void cptBufferCallback();
	void handleCallback(u_int16_t *buffer);

	void endPeriodicVib(int eventID);
	void handlePeriodicCall(VibrationTemplate *vib);
	void playPtr(VibrationTemplate *vib);
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_H_
