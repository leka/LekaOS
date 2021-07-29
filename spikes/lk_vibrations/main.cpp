// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"

#include "CoreAudio.h"
#include "CoreVibration.h"
#include "HelloWorld.h"
#include "LKCoreSTM32Hal.h"
//#include "VibrationsUtils.h"

using namespace leka;
using namespace std::chrono;

// uint16_t _outBuffer[OUT_BUFF_SIZE] = {0};

mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

LKCoreSTM32Hal hal;
CoreDACTimer coreDACTimer(hal);
CoreDAC coreDAC(hal);

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
	printf("\n\nCardiac coherence demo!!\n");
	VibrationTemplate vib(5s, 90, 0.2, VibrationEnvelope::Sawtooth);
	coreVib.playPeriodically(vib, 5s, 6);
	while (coreVib.isPlayingPeriodically()) {
		ThisThread::sleep_for(50ms);
	}
}

void countVibsDemo()
{
	printf("\n\nCount vibrations demo!!\n");
	VibrationTemplate vib(1s, 90, 0.2, VibrationEnvelope::Window);
	for (int i = 0; i < 10; i++) {
		int reps = rand() % 9 + 1;
		coreVib.playPeriodically(vib, 500ms, reps);

		while (coreVib.isPlayingPeriodically()) {
			ThisThread::sleep_for(50ms);
		}
		printf("The number of times to guess was: %d\n", reps);
		ThisThread::sleep_for(2s);
	}
}

void vibTypeDemo()
{
	printf("\n\nVib type demo!!\n");
	int numFreqs				   = 6;
	std::array<uint32_t, 10> freqs = {50, 90, 100, 110, 128, 140};

	int numAmplis				 = 3;
	std::array<float, 10> amplis = {0.4, 0.6, 0.8};

	for (int i = 0; i < numFreqs; ++i) {
		printf("freq: %lu\n", freqs.at(i));
		for (int j = 0; j < numAmplis; ++j) {
			VibrationTemplate vib(1s, freqs.at(i), amplis.at(j), VibrationEnvelope::Window);
			coreVib.play(vib);
			while (coreVib.isPlaying()) {
				ThisThread::sleep_for(50ms);
			}
			printf("\tampli: %f\n", amplis.at(j));
			ThisThread::sleep_for(500ms);
		}
		ThisThread::sleep_for(2s);
	}
}

void testFloatWorkflow(uint16_t *outBuffer)
{
	float sinBuffer[16];
	// printf("Float#############\n");

	// random values
	float vibAmpli = 0.1;
	float sinAmpli = 0.4 * 0xFFF;
	float env	   = 0.3;
	float offset   = 0.45 * 0xFFF;

	// first calculations of sinus
	// ampli is calculated here
	for (uint32_t i = 0; i < 16; ++i) {
		sinBuffer[i] = vibAmpli * sinAmpli * sin(i * 2.0 * M_PI / 16);
		//	printf("%.3f\n", sinBuffer[i]);
	}

	// consider 1000 periods

	for (uint32_t i = 0; i < 10000; ++i) {
		for (uint32_t j = 0; j < 16; ++j) {
			outBuffer[j] = env * sinBuffer[j] + offset;
		}
	}
}

void testIntWorkflow(uint16_t *outBuffer)
{
	int16_t sinBuffer[16];
	// printf("Int#############\n");

	// random values
	float vibAmpli	  = 0.1;
	uint16_t sinAmpli = 0.4 * 0xFFF;
	float env		  = 0.3;
	uint16_t offset	  = 0.45 * 0xFFF;

	// first calculations of sinus
	// ampli is calculated here
	for (uint32_t i = 0; i < 16; ++i) {
		sinBuffer[i] = vibAmpli * sinAmpli * sin(i * 2.0 * M_PI / 16);
		// printf("%d\n", sinBuffer[i]);
	}

	// consider 1000 periods
	for (uint32_t i = 0; i < 10000; ++i) {
		for (uint32_t j = 0; j < 16; ++j) {
			outBuffer[j] = sinBuffer[j] * env + offset;
		}
	}
}

void testUintWorkflow(uint16_t *outBuffer)
{
	uint16_t sinBuffer[16];
	// printf("Uint#############\n");

	float vibAmpli = 0.1;
	float sinAmpli = 0.4;
	float env	   = 0.3;
	float offset   = 0.45;

	// first calculations of sinus]
	for (uint32_t i = 0; i < 16; ++i) {
		double tmp	 = sinAmpli * vibAmpli * sin(i * 2.0 * M_PI / 16) + offset;
		sinBuffer[i] = static_cast<uint16_t>(tmp * UINT16_MAX);
		// printf("%lu\n", sinBuffer[i]);
	}

	// consider 1000 periods
	for (uint32_t i = 0; i < 10000; ++i) {
		for (uint32_t j = 0; j < 16; ++j) {
			outBuffer[j] = static_cast<uint16_t>((sinBuffer[j] - static_cast<uint16_t>(offset * UINT16_MAX)) * env +
												 static_cast<uint16_t>(offset * UINT16_MAX)) >>
						   4;
		}
	}
}

void testWorkflows()
{
	uint16_t buff_1[16];
	uint16_t buff_2[16];
	uint16_t buff_3[16];

	Timer t;
	t.start();

	auto start = t.elapsed_time();
	testFloatWorkflow(buff_1);
	auto end = t.elapsed_time();
	printf("Float time ns: \n%lld us\n", chrono::duration_cast<chrono::microseconds>(end - start).count());

	start = t.elapsed_time();
	testIntWorkflow(buff_2);
	end = t.elapsed_time();
	printf("Int time ns: \n%lld us\n", chrono::duration_cast<chrono::microseconds>(end - start).count());

	start = t.elapsed_time();
	testUintWorkflow(buff_3);
	end = t.elapsed_time();
	printf("Uint time ns: \n%lld us\n", chrono::duration_cast<chrono::microseconds>(end - start).count());

	printf("Supposedly equal :\n");
	for (int i = 0; i < 16; ++i) {
		printf("%d\t%d\t%d\n", buff_1[i], buff_2[i], buff_3[i]);
	}
}

auto main() -> int
{
	printf("\n\nStarting vibration process, hold your kids\n\n\n");
	coreVib.initialize(2048);

	testWorkflows();

	// printf("\n\nEnd of demos\n\n");
	while (true) {
		// cardiacCoherenceDemo();
		// countVibsDemo();
		// vibTypeDemo();
		rtos::ThisThread::sleep_for(4s);
	}

	coreVib.deInit();
}

// float duration	   = 2.0;
// uint32_t frequency = 128;
// float amplitude	   = 0.1;

// VibrationTemplate vib1(duration, frequency, amplitude, false);
// VibrationTemplate vib2(1, 200, 0.3, false);
// VibrationTemplate vib3(0.5, 128, 0.1, false);
// VibrationTemplate pause_1s(1, 1, 0, false);
// VibrationTemplate pause_2s(2, 1, 0, false);
// VibrationTemplate pause_500ms(0.5, 1, 0, false);

// vector<VibrationTemplate> sequence;
// sequence.push_back(vib1);
// sequence.push_back(pause_2s);

// sequence.push_back(vib2);
// sequence.push_back(pause_1s);
// sequence.push_back(vib2);
// sequence.push_back(pause_1s);

// sequence.push_back(vib3);
// sequence.push_back(pause_500ms);
// sequence.push_back(vib3);
// sequence.push_back(pause_500ms);
// sequence.push_back(vib3);
// sequence.push_back(pause_500ms);
// sequence.push_back(vib3);
// sequence.push_back(pause_500ms);

// coreVib.play(vib1);