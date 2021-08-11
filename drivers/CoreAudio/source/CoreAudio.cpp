// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreAudio.h"

namespace leka {

uint16_t CoreAudio::_waveBuffer[512];

CoreAudio::CoreAudio(LKCoreSTM32HalBase &hal, CoreDAC &dac, CoreDACTimer &timer, rtos::Thread &thread,
					 events::EventQueue &eventQueue)
	: playing(false),
	  _hal(hal),
	  _coreDac(dac),
	  _coreTimer(timer),
	  _thread(thread),
	  _eventQueue(eventQueue),
	  _volume(100),
	  _eofFlag(EndOfFileState::NotFinished)
{
	_thread.start({&_eventQueue, &events::EventQueue::dispatch_forever});
}

}	// namespace leka
