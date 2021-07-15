// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_INTERFACE_DRIVER_TOUCH_READER_H_
#define _LEKA_OS_INTERFACE_DRIVER_TOUCH_READER_H_

namespace leka::interface {

class TouchReader
{
  public:
	virtual ~TouchReader() = default;

	virtual void initialize()					= 0;
	virtual auto readSensorsValues() -> uint8_t = 0;

	virtual void registerOnTouchCallback(mbed::Callback<void()> on_touch_callback) = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_INTERFACE_DRIVER_TOUCH_READER_H_
