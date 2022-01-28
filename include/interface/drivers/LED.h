// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_LED_H_
#define _LEKA_OS_INTERFACE_DRIVER_LED_H_

#include <cstdint>

#include "RGB.h"

namespace leka::interface {

class LED
{
  public:
	virtual ~LED() = default;

	virtual auto setColor(RGB color) -> void = 0;
	virtual auto showColor() -> void		 = 0;
	virtual auto hideColor() -> void		 = 0;

	virtual auto isOn() -> bool = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_LED_H_
