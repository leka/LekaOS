// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreIMULSM6DSOX.h"
#include "RGB.h"
#include "interface/drivers/LED.h"

#pragma once
namespace motion::record {
namespace card {
	inline constexpr auto ODR_12_5HZ = 0x0023;
	inline constexpr auto ODR_26HZ	 = 0x0024;
	inline constexpr auto ODR_52HZ	 = 0x0025;
	inline constexpr auto ODR_104HZ	 = 0x0027;

}	// namespace card
class ODRCard
{
  public:
	ODRCard(uint16_t id_card, leka::RGB color, lsm6dsox_odr_xl_t XL_ODR, lsm6dsox_odr_g_t GY_ODR,
			std::chrono::milliseconds time_odr)
		: _id_card(id_card), _color(color), _XL_ODR(XL_ODR), _GY_ODR(GY_ODR), _time_odr(time_odr)
	{
	}

	[[nodiscard]] auto getId() const -> uint16_t { return this->_id_card; }
	[[nodiscard]] auto getXlODR() const -> lsm6dsox_odr_xl_t { return this->_XL_ODR; }
	[[nodiscard]] auto getGyODR() const -> lsm6dsox_odr_g_t { return this->_GY_ODR; }
	[[nodiscard]] auto getColor() const -> leka::RGB { return this->_color; }
	[[nodiscard]] auto getTimeODR() const -> std::chrono::milliseconds { return this->_time_odr; }

  private:
	uint16_t _id_card;
	leka::RGB _color;
	lsm6dsox_odr_xl_t _XL_ODR;
	lsm6dsox_odr_g_t _GY_ODR;
	std::chrono::milliseconds _time_odr;
};

template <typename T1, typename T2>
void updateODR(leka::interface::LED &led, leka::CoreIMULSM6DSOX &coreimu, const leka::RGB &color,
			   lsm6dsox_odr_xl_t XL_ODR, lsm6dsox_odr_g_t GY_ODR, std::chrono::milliseconds &time_odr,
			   std::chrono::milliseconds odr_value)
{
	led.setColor(color);
	led.show();
	coreimu.setAccelRate(static_cast<T1>(XL_ODR));
	coreimu.setGyrRate(static_cast<T2>(GY_ODR));
	time_odr = odr_value;
}

}	// namespace motion::record
