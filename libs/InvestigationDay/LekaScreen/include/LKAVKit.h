// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LKAVKIT_H_
#define _LEKA_OS_LKAVKIT_H_

#include "DrawingTools.h"
#include "LKCoreAV.h"

namespace leka {
class LKAVKit
{
  public:
	LKAVKit(LKCoreAV &lk_core_av, LCD_Model lcd_model);

	void turnOff();

	void clear(uint32_t ColorIndex = 0xFFFFFFFF);
	void screenSaver(std::chrono::seconds duration_sec = std::chrono::seconds(10));

	void displayImage(FIL *file);

	// NOT IMPLEMENTED

	void playVideo();
	void playSound();
	void writeText();

  private:
	LKCoreAV &_lk_core_av;
};
}	// namespace leka

#endif	 // _LEKA_OS_LKAVKIT_H_
