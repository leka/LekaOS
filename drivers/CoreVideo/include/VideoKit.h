#ifndef _LEKA_OS_LIB_VideoKit_H_
#define _LEKA_OS_LIB_VideoKit_H_

#include <chrono>

#include "rtos/ThisThread.h"

#include "CoreDMA2D.h"
#include "CoreDSI.h"
#include "CoreJPEG.h"
#include "CoreLCD.h"
#include "CoreLCDDriverOTM8009A.h"
#include "CoreLTDC.h"
#include "CoreSDRAM.h"
#include "LKCoreSTM32Hal.h"

namespace leka {

class VideoKit
{
  public:
	VideoKit();

	void clear();
	void display();

  private:
	LKCoreSTM32Hal _hal;
	CoreSDRAM _coresdram;

	// ltdc + dsi + lcd screen
	CoreLTDC _coreltdc;
	CoreDSI _coredsi;
	CoreLCDDriverOTM8009A _coreotm;
	CoreLCD _corelcd;

	// peripherals
	CoreDMA2D _coredma2d;
	CoreJPEG _corejpeg;

	rtos::Kernel::Clock::time_point _last_time = rtos::Kernel::Clock::now();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VideoKit_H_
