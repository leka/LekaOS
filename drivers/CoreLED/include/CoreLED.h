// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_DRIVER_CORELED_H_
#define _LEKA_OS_DRIVER_CORELED_H_

#include "CoreSPI.h"

#include "interface/drivers/LED.h"

namespace leka {

class CoreLED : public interface::LED
{
  public:
    explicit CoreLED(CoreSPI &corespi) : _corespi {corespi} {};

	// Getters & setters

	auto setBeltColor(LEDColor color) -> void override;
	auto getBeltColor() -> LEDColor override;
	[[nodiscard]] auto getState() const-> bool;
	auto setState(bool state) -> void;

    auto showColor() -> void override;
    auto turnOn() -> void override;
    auto turnOff() -> void override;


  private:

    LEDColor belt_current_color;
	bool belt_is_on;
	CoreSPI &_corespi;

};

}   // namespace leka

#endif   // _LEKA_OS_DRIVER_CORELED_H_
