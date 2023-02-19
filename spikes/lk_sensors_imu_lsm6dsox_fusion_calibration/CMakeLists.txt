# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_executable(spike_lk_sensors_imu_lsm6dsox_fusion_calibration)

target_include_directories(spike_lk_sensors_imu_lsm6dsox_fusion_calibration
	PRIVATE
	.
)

target_sources(spike_lk_sensors_imu_lsm6dsox_fusion_calibration
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_sensors_imu_lsm6dsox_fusion_calibration
	CoreIMU
	CoreI2C
	Fusion
)
