// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_SCREEN_H_
#define _LEKA_OS_LIB_SCREEN_H_

#include "rtos/ThisThread.h"

#include "Display.h"
#include "DrawingTools.h"
#include "decode.h"
#include "otm8009a_conf.h"

class Screen
{
  public:
	Screen();
	~Screen() {};

	void start(void);

	void clear(uint32_t ColorIndex);

	void showFace();

  private:
};

#endif
