// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_ODR_CARD_H_
#define _LEKA_OS_SPIKE_ODR_CARD_H_

#include <chrono>
#include <string>

#include "CoreIMULSM6DSOX.h"

#pragma once

using namespace std::literals::chrono_literals;

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
	ODRCard(auto id_card, std::string name_img, lsm6dsox_odr_xl_t XL_ODR, lsm6dsox_odr_g_t GY_ODR, auto time_odr)
		: _id_card(id_card), _time_odr(time_odr), _XL_ODR(XL_ODR), _GY_ODR(GY_ODR)
	{
		this->_img_path = "/fs/home/img/shapesRecognition/IMU_ODR/" + name_img + ".jpg";
	}

	[[nodiscard]] auto getId() const -> uint16_t { return this->_id_card; }
	auto getXlODR() -> lsm6dsox_odr_xl_t { return this->_XL_ODR; }
	auto getGyODR() -> lsm6dsox_odr_g_t { return this->_GY_ODR; }
	auto getImgPath() -> std::string { return this->_img_path; }
	auto getTimeODR() -> std::chrono::milliseconds { return this->_time_odr; }

  private:
	uint16_t _id_card;
	std::string _img_path;
	lsm6dsox_odr_g_t _GY_ODR;
	lsm6dsox_odr_xl_t _XL_ODR;
	std::chrono::milliseconds _time_odr;
};

template <typename T1, typename T2>
void updateODR(auto &videokit, auto &coreimu, std::string img_path, auto XL_ODR, auto GY_ODR, auto &time_odr,
			   auto odr_value)
{
	videokit.displayImage(img_path);
	coreimu.setAccelRate(static_cast<T1>(XL_ODR));
	coreimu.setGyrRate(static_cast<T2>(GY_ODR));
	time_odr = odr_value;
}

}	// namespace motion::record

#endif