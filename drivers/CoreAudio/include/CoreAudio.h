// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_AUDIO_H_
#define _LEKA_OS_LIB_AUDIO_H_


#include "LKCoreSTM32HalBase.h"
#include "LKCoreFatFs.h"

#include "CoreDACTimer.h"
namespace leka {

class CoreAudio 
{
  public:

    CoreAudio();
    void initialize();
    void playFile(FIL* file);
    void pause();
    void resume();
    void stop();
    void setVolume(int volume);


  private:
    LKCoreSTM32HalBase &_hal;
    // CoreDACBase &_dac;
    // CoreDMABase &_dma;
    CoreDACTimer &_timer;


};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_AUDIO_H_
