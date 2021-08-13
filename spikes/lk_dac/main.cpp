// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"
#include <sys/types.h>

#include "CoreDAC.h"
#include "LKCoreSTM32Hal.h"

using namespace leka;

// Components
LKCoreSTM32Hal hal;
DACDriver coreDac(hal);
DACTimer coreTimer_6(hal, DACTimer::HardWareBasicTimer::BasicTimer6);
DACTimer coreTimer_7(hal, DACTimer::HardWareBasicTimer::BasicTimer7);

// Globals
uint32_t _countCbCalls					 = 0;
bool _vibEnded							 = false;
DACTimer::HardWareBasicTimer _currentTim = DACTimer::HardWareBasicTimer::BasicTimer6;

// Preset values
constexpr uint32_t _samplingRate_hertz = 44100;
constexpr int _vibDuration_seconds	   = 2;
constexpr int _outBufferSize_samples   = 512;

/**
 * @brief This function handles DMA1 stream5 global interrupt.
 */
extern "C" {
void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coreDac.getHandle().DMA_Handle1);
}
}

void fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t sinFreq, uint32_t samplingRate,
						   uint16_t maxValue, uint16_t minValue)
{
	uint32_t samplesPerPeriod = (samplingRate / sinFreq);

	for (uint32_t i = 0; i < bufferSize; ++i) {
		double tmp = 0.5 * sin(i * 2.0 * M_PI / samplesPerPeriod) + 0.5;
		tmp *= maxValue - minValue;
		buffer[i] = static_cast<uint16_t>(tmp + minValue);
	}
}

void callbackTest(DAC_HandleTypeDef *)
{
	static const int samplesPerHalfBuff		= _outBufferSize_samples / 2;
	static const uint32_t nbCallsInDuration = _vibDuration_seconds * _samplingRate_hertz / samplesPerHalfBuff;
	if (_countCbCalls <= nbCallsInDuration) {
		_countCbCalls++;
	}
	if (_countCbCalls == nbCallsInDuration) {
		_vibEnded = true;

		if (_currentTim == DACTimer::HardWareBasicTimer::BasicTimer6) {
			coreTimer_6.stop();
		} else {
			coreTimer_7.stop();
		}
	}
}

void startSound(DACTimer::HardWareBasicTimer tim)
{
	_countCbCalls = 0;
	_vibEnded	  = false;
	if (tim == DACTimer::HardWareBasicTimer::BasicTimer6) {
		coreTimer_6.start();
		_currentTim = DACTimer::HardWareBasicTimer::BasicTimer6;
	} else if (tim == DACTimer::HardWareBasicTimer::BasicTimer7) {
		coreTimer_7.start();
		_currentTim = DACTimer::HardWareBasicTimer::BasicTimer7;
	}
}

/**
 * @brief Tests DAC and Timer
 * Will make the haptic actuator vibrate at a given frequency during 2 phases, one per timer
 * @return int
 */
auto main() -> int
{
	mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	std::array<uint16_t, 512> outBuff {};
	const uint32_t sinFreq = 440;
	const uint16_t maxVal  = 0x999;
	const uint16_t minVal  = 0x666;
	fillBufferWithSinWave(outBuff.data(), outBuff.size(), sinFreq, _samplingRate_hertz, maxVal, minVal);
	auto outSpan = lstd::span {outBuff.data(), outBuff.size()};

	log_info("Hello, investigation day!");

	// Init
	coreTimer_6.initialize(_samplingRate_hertz);
	coreTimer_7.initialize(_samplingRate_hertz);

	coreDac.initialize(coreTimer_6, &callbackTest, &callbackTest);
	coreDac.start(outSpan);

	// start
	log_info("First timer for %ds", _vibDuration_seconds);
	startSound(DACTimer::HardWareBasicTimer::BasicTimer6);

	while (!_vibEnded) {
		rtos::ThisThread::sleep_for(50ms);
	}
	log_info("Pause");
	rtos::ThisThread::sleep_for(2s);

	coreDac.linkNewTimer(coreTimer_7);	 // change timer associated to DAC

	log_info("Second timer for %ds", _vibDuration_seconds);
	startSound(DACTimer::HardWareBasicTimer::BasicTimer7);

	while (!_vibEnded) {
		rtos::ThisThread::sleep_for(50ms);
	}

	log_info("End of DAC and Timer test!");
}
