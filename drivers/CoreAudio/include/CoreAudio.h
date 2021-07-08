// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_AUDIO_H_
#define _LEKA_OS_LIB_AUDIO_H_


#include "LKCoreSTM32HalBase.h"
#include "LKCoreFatFs.h"

#include "CoreDACTimer.h"
#include "CoreDAC.h"
#include "WavFile.h"
#include "WavReader.h"




namespace leka {

class CoreAudio 
{
  public:

    CoreAudio(LKCoreSTM32HalBase &hal,CoreDAC &dac, CoreDACTimer &timer);
    void playFile(FIL* file);
    void pause();
    void resume();
    void stop();
    void setVolume(float volume) {_volume = volume; };

    void fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t frequency, uint32_t samplingRate = 44100, uint16_t maxValue = 0xFFFF, uint16_t minValue = 0x0);
    void fillBufferWithSquare(uint16_t *buffer, uint32_t bufferSize,uint16_t maxValue, uint16_t minValue);
  private:
    static uint16_t _waveBuffer[512];

    LKCoreSTM32HalBase &_hal;
    CoreDAC &_coreDac;
    CoreDACTimer &_coreTimer;
    float _volume;

    bool _playing;

    void _initialize(float frequency);
    void _align12bR(uint16_t *buffer, uint16_t length);
    void _scaleToVolume(uint16_t *buffer, uint16_t length);

};

}	// namespace leka



#endif	 // _LEKA_OS_LIB_AUDIO_H_
