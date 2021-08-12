/*
// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"
#include <string>

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "CoreAudio.h"
#include "CoreSDRAM.h"
#include "FATFileSystem.h"
#include "FileManager.h"
#include "LKCoreSTM32Hal.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;

LKCoreSTM32Hal hal;
CoreDAC coreDAC(hal);
CoreSDRAM coreSDRAM(hal);

VideoKit screen(hal);
VideoKit_DeclareIRQHandlers(screen);

rtos::Thread video_thread;

extern "C" {
void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coreDAC.getHandle().DMA_Handle1);
}
}

struct ThreadArgs {
	gfx::Video* video;
	gfx::Image* image;
};

void videoThread(ThreadArgs *args)
{
	// play the video in a loop
	while (1) {
		screen.draw(*args->video);
		args->video->nextFrame();

		if (args->video->hasEnded())
			break;

		screen.display();
	}
}

auto main() -> int
{
	leka::FileManager sd_card;

	mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });
	log_info("\n\tHello, investigation day!");

	rtos::Thread audioThread;
	events::EventQueue audioEventQueue;

	CoreDACTimer coreDACTimer(hal, CoreDACTimer::HardWareBasicTimer::BasicTimer6);
	CoreAudio coreAudio(hal, coreDAC, coreDACTimer, audioThread, audioEventQueue);

	coreSDRAM.initialize();
	screen.initialize();

	gfx::Video video("/fs/assets/video/20fps_low10.avi");
	gfx::Image image("/fs/assets/images/Leka/logo.jpg");
	ThreadArgs thread_args{
		&video,
		&image
	};
	// start video thread that plays a video
	video_thread.start(mbed::callback(&videoThread, &thread_args));

	// video_thread.join();

	FIL file;
	std::string filename = "test-voix.wav";	  // sawtooth10_44ksamp_mono_5sec.wav

	while (true) {
		coreAudio.setVolume(30);

		filename = "test-voix-100Hzmin.wav";
		if (f_open(&file, filename.c_str(), FA_READ) == FR_OK) {
			log_info("\n\tFile %s did open!!", filename.c_str());
			coreAudio.playFile(&file);
		} else {
			log_info("\n\tCould not open file");
		}

		while (coreAudio.isPlaying()) {
			rtos::ThisThread::yield();
		}

		f_close(&file);


		coreAudio.setVolume(80);
		filename = "fur-elise-100Hzmin.wav";   // sawtooth10_44ksamp_mono_5sec.wav
		//filename = "sin220_44ksamp_mono_10sec.wav";   // sawtooth10_44ksamp_mono_5sec.wav

		if (f_open(&file, filename.c_str(), FA_READ) == FR_OK) {
			log_info("\n\tFile %s did open!!", filename.c_str());
			coreAudio.playFile(&file);
		} else {
			log_info("\n\tCould not open file");
		}

		while (coreAudio.isPlaying()) {
			rtos::ThisThread::yield();
		}

		f_close(&file);

		rtos::ThisThread::sleep_for(1s);
	}
}
*/

// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "mbed.h"
#include <string>

#include "events/EventQueue.h"
#include "rtos/Thread.h"

#include "CoreAudio.h"
#include "CoreSDRAM.h"
#include "FATFileSystem.h"
#include "FileManager.h"
#include "LKCoreSTM32Hal.h"
#include "SDBlockDevice.h"
#include "VideoKit.h"

using namespace leka;

LKCoreSTM32Hal hal;
CoreDAC coreDac(hal);
CoreSDRAM coreSDRAM(hal);

VideoKit screen(hal);
VideoKit_DeclareIRQHandlers(screen);

CoreDACTimer coreTimer_6(hal, CoreDACTimer::HardWareBasicTimer::BasicTimer6);
CoreDACTimer coreTimer_7(hal, CoreDACTimer::HardWareBasicTimer::BasicTimer7);

// Globals
uint32_t _countCbCalls						 = 0;
bool _vibEnded								 = false;
CoreDACTimer::HardWareBasicTimer _currentTim = CoreDACTimer::HardWareBasicTimer::BasicTimer6;

// Preset values
constexpr uint32_t _samplingRate_hertz = 44100;
constexpr int _vibDuration_seconds	   = 2;
constexpr int _outBufferSize_samples   = 512;

rtos::Thread video_thread;

extern "C" {
void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coreDac.getHandle().DMA_Handle1);
}
}

struct ThreadArgs {
	gfx::Video *video;
	gfx::Image *image;
};

void videoThread(ThreadArgs *args)
{
	// play the video in a loop
	while (1) {
		screen.draw(*args->video);
		args->video->nextFrame();

		if (args->video->hasEnded()) break;

		screen.display();
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

		if (_currentTim == CoreDACTimer::HardWareBasicTimer::BasicTimer6) {
			coreTimer_6.stop();
		} else {
			coreTimer_7.stop();
		}
	}
}

void startSound(CoreDACTimer::HardWareBasicTimer tim)
{
	_countCbCalls = 0;
	_vibEnded	  = false;
	if (tim == CoreDACTimer::HardWareBasicTimer::BasicTimer6) {
		coreTimer_6.start();
		_currentTim = CoreDACTimer::HardWareBasicTimer::BasicTimer6;
	} else if (tim == CoreDACTimer::HardWareBasicTimer::BasicTimer7) {
		coreTimer_7.start();
		_currentTim = CoreDACTimer::HardWareBasicTimer::BasicTimer7;
	}
}

/**
 * @brief Tests DAC and Timer
 * Will make the haptic actuator vibrate at a given frequency during 2 phases, one per timer
 * @return int
 */
auto main() -> int
{
	leka::FileManager sd_card;

	mbed::DigitalOut audio_enable(SOUND_ENABLE, 1);
	static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

	leka::logger::set_print_function([](const char *str, size_t size) { serial.write(str, size); });

	coreSDRAM.initialize();
	screen.initialize();

	gfx::Video video("/fs/assets/video/20fps_low10.avi");
	gfx::Image image("/fs/assets/images/Leka/logo.jpg");
	ThreadArgs thread_args {&video, &image};
	// start video thread that plays a video
	video_thread.start(mbed::callback(&videoThread, &thread_args));

	// video_thread.join();

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
	startSound(CoreDACTimer::HardWareBasicTimer::BasicTimer6);

	while (!_vibEnded) {
		rtos::ThisThread::sleep_for(50ms);
	}
	log_info("Pause");
	rtos::ThisThread::sleep_for(2s);

	coreDac.linkNewTimer(coreTimer_7);	 // change timer associated to DAC

	log_info("Second timer for %ds", _vibDuration_seconds);
	startSound(CoreDACTimer::HardWareBasicTimer::BasicTimer7);

	while (!_vibEnded) {
		rtos::ThisThread::sleep_for(50ms);
	}

	log_info("End of DAC and Timer test!");
}
