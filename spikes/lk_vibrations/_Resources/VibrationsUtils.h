
// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_VIBRATION_UTILS_H_
#define _LEKA_OS_SPIKE_VIBRATION_UTILS_H_

#define SIN_PERIOD_SIZE 16
#define SIN_FREQUENCY	128
#define OUT_BUFF_SIZE	512
#define SAMPLING_RATE	2048
#define DAC_MAX_VAL		0xAAA

#include <array>
#include <cmath>
#include <cstdint>

#include "rtos/ThisThread.h"

#include "CoreDAC.h"
#include "CoreDACTimer.h"

using namespace leka;

void fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t frequency, uint32_t samplingRate = 44100,
						   uint16_t maxValue = 0xFFFF, uint16_t minValue = 0x0);

void fillBufferWithEnvelop(uint16_t *buffer, uint32_t bufferSize, EnvelopType eType, uint16_t maxVal = 0xFFFF);

void fillEnvelop(uint16_t *buffer, uint32_t bufferSize, uint16_t envelopMaxVal);

void playPattern(uint16_t *buffer, uint32_t bufferSize, EnvelopType eType, uint16_t numTimes, CoreDACTimer &dtimer,
				 CoreDAC &dac);

void halfBufferCallback(DAC_HandleTypeDef *hdac);
void cptBufferCallback(DAC_HandleTypeDef *hdac);

auto createSinWavePeriod(uint16_t *sinBuffer, uint32_t samplesPerPeriod) -> uint32_t;

void playFlatVibration(Vibration vib);

extern int32_t _endOfVibCounter;
extern uint32_t _samplesPerPeriod;
extern bool _vibrationFinished;

#endif	 // _LEKA_OS_SPIKE_VIBRATION_UTILS_H_