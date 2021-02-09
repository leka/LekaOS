// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LKAVKIT_H_
#define _LEKA_OS_LKAVKIT_H_

#include "DrawingTools.h"
#include "LKCoreAV.h"
#include "Screensaver.h"

namespace leka {
class LKAVKit
{
  public:
	LKAVKit(LKCoreAV &lk_core_av);

	void turnOff();

	void clear(uint32_t color = 0xFFFFFFFF);
	void screensaver();

	void displayImage(FIL *file);

	// NOT IMPLEMENTED

	void playVideo();
	void playSound();
	void writeText();

  private:
	LKCoreAV &_lk_core_av;

	Draw _draw;
	Screensaver _screensaver;
};
}	// namespace leka

#endif	 // _LEKA_OS_LKAVKIT_H_
