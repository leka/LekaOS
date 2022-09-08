// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>

namespace leka {
enum odr_g_t
{
	GY_ODR_OFF	  = 0,
	GY_ODR_12Hz5  = 1,
	GY_ODR_26Hz	  = 2,
	GY_ODR_52Hz	  = 3,
	GY_ODR_104Hz  = 4,
	GY_ODR_208Hz  = 5,
	GY_ODR_417Hz  = 6,
	GY_ODR_833Hz  = 7,
	GY_ODR_1667Hz = 8,
	GY_ODR_3333Hz = 9,
	GY_ODR_6667Hz = 10,
};

enum fs_g_t
{
	GY_FS_250dps  = 0,
	GY_FS_125dps  = 1,
	GY_FS_500dps  = 2,
	GY_FS_1000dps = 4,
	GY_FS_2000dps = 6,
};
}	// namespace leka

namespace leka::interface {

class Gyroscope
{
  public:
	virtual ~Gyroscope() = default;

	virtual auto getDataX_mdps() -> float = 0;
	virtual auto getDataY_mdps() -> float = 0;
	virtual auto getDataZ_mdps() -> float = 0;

	virtual auto getDataX_dps() -> float = 0;
	virtual auto getDataY_dps() -> float = 0;
	virtual auto getDataZ_dps() -> float = 0;

	virtual void getData_mdps(std::array<float, 3> &gy_data) = 0;
	virtual void getData_dps(std::array<float, 3> &gy_data)	 = 0;

	virtual void setDataRate(odr_g_t odr_gy) = 0;
	virtual auto getDataRateGyr() -> int32_t = 0;

	virtual void SetFullScale(fs_g_t fs_gy) = 0;

	virtual void TurnOffGyr() = 0;

  private:
};

}	// namespace leka::interface
