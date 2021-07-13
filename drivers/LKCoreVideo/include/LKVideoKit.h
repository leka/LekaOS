#ifndef _LEKA_OS_LIB_LKVIDEOKIT_H_
#define _LEKA_OS_LIB_LKVIDEOKIT_H_

#include <chrono>

#include "rtos/ThisThread.h"

#include "LKCoreSTM32Hal.h"
#include "LKCoreSDRAM.h"
#include "LKCoreDSI.h"
#include "LKCoreLCD.h"
#include "LKCoreLCDDriverOTM8009A.h"
#include "LKCoreLTDC.h"
#include "LKCoreJPEG.h"
#include "LKCoreDMA2D.h"

namespace leka {

class LKVideoKit {
  public:
	LKVideoKit();

	void clear();
	void display();

  private:
	LKCoreSTM32Hal _hal;
	LKCoreSDRAM _coresdram;

	// screen + dsi + ltdc
	LKCoreDSI _coredsi;
	LKCoreLTDC _coreltdc;
	LKCoreLCDDriverOTM8009A _coreotm;
	LKCoreLCD _corelcd;

	// peripherals
	LKCoreDMA2D _coredma2d;
	LKCoreJPEG _corejpeg;

	rtos::Kernel::Clock::time_point _last_time = rtos::Kernel::Clock::now();
};

}

#endif // _LEKA_OS_LIB_LKVIDEOKIT_H_
