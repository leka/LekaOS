
// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_VIBRATION_UTILS_H_
#define _LEKA_OS_SPIKE_VIBRATION_UTILS_H_

#define SIN_PERIOD_SIZE 16
#define SIN_FREQUENCY	128
#define OUT_BUFF_SIZE	256
#define SAMPLING_RATE	2048
#define DAC_MAX_VAL		0xFFF

#include <array>
#include <cmath>
#include <cstdint>

enum EnvelopType
{
	Window,
	Sawtooth,
	Click,	 // small fast
	Ramp,	 // Up 2/3 of the time and down 1/3
	Buzz,	 // round
	Pulse	 // multiple maximums
};

void fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t frequency, uint32_t samplingRate = 44100,
						   uint16_t maxValue = 0xFFFF, uint16_t minValue = 0x0);

void fillBufferWithEnvelop(uint16_t *buffer, uint32_t bufferSize, EnvelopType eType, uint16_t maxVal = 0xFFFF);

void fillEnvelop(uint16_t *buffer, uint32_t bufferSize, uint16_t envelopMaxVal);

#endif	 // _LEKA_OS_SPIKE_VIBRATION_UTILS_H_