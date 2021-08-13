// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

#include "CoreVibration.h"
#include "HelloWorld.h"
#include "LKCoreSTM32Hal.h"
#include "testUtils.h"
//#include "VibrationsUtils.h"

using namespace leka;
using namespace std::chrono;

// uint16_t _outBuffer[OUT_BUFF_SIZE] = {0};

mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

LKCoreSTM32Hal hal;
DACTimer coreDACTimer(hal, DACTimer::HardwareBasicTimer::BasicTimer6);
DACDriver coreDAC(hal);

rtos::Thread vibrationsThread;
events::EventQueue vibrationsEventQueue;

CoreVibration coreVib(hal, coreDAC, coreDACTimer, vibrationsThread, vibrationsEventQueue);

/**
 * @brief This function handles DMA1 stream5 global interrupt.
 */
extern "C" {
void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coreDAC.getHandle().DMA_Handle1);
}
}

void cardiacCoherenceDemo()
{
	log_info("\n\nCardiac coherence demo!!\n");
	VibrationTemplate vib(5s, 100, 0.1, VibrationEnvelope::EnvelopType::Triangle);
	coreVib.playPeriodically(vib, 5s, 6);
	while (coreVib.isPlayingPeriodically()) {
		ThisThread::sleep_for(50ms);
	}
}

void countVibsDemo()
{
	log_info("\n\nCount vibrations demo!!\n");
	VibrationTemplate vib(1s, 100, 0.1, VibrationEnvelope::EnvelopType::Triangle);
	for (int i = 0; i < 10; i++) {
		int reps = rand() % 9 + 1;
		coreVib.playPeriodically(vib, 500ms, reps);

		while (coreVib.isPlayingPeriodically()) {
			ThisThread::sleep_for(50ms);
		}
		log_info("The number of times to guess was: %d\n", reps);
		ThisThread::sleep_for(2s);
	}
}

void vibTypeDemo()
{
	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	log_info("\n\nVib type demo!!\n");
	int numFreqs				   = 7;
	std::array<uint32_t, 10> freqs = {50, 90, 96, 100, 110, 128, 140};

	int numAmplis				 = 3;
	std::array<float, 10> amplis = {0.4, 0.6, 0.8};

	for (int i = 0; i < numFreqs; ++i) {
		log_info("freq: %lu\n", freqs.at(i));
		for (int j = 0; j < numAmplis; ++j) {
			VibrationTemplate vib(1s, freqs.at(i), amplis.at(j), VibrationEnvelope::EnvelopType::Window);
			coreVib.play(vib);
			while (coreVib.isPlaying()) {
				ThisThread::sleep_for(50ms);
			}
			log_info("\tampli: %f\n", amplis.at(j));
			ThisThread::sleep_for(500ms);
		}
		ThisThread::sleep_for(2s);
	}
}

auto main() -> int
{
	log_info("\n\nStarting vibration process, hold your kids");
	coreVib.initialize(2048);

	VibrationTemplate vib(1s, 100, 0.2, VibrationEnvelope::EnvelopType::Triangle);

	while (true) {
		coreVib.play(vib);
		while (coreVib.isPlaying()) {
			ThisThread::sleep_for(50ms);
		}
		// cardiacCoherenceDemo();
		// countVibsDemo();
		// vibTypeDemo();
		rtos::ThisThread::sleep_for(4s);
	}

	coreVib.deInit();
}