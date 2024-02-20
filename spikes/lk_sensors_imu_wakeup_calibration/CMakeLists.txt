# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_sensors_imu_wakeup_calibration)

target_include_directories(spike_lk_sensors_imu_wakeup_calibration
	PRIVATE
	.
)

target_sources(spike_lk_sensors_imu_wakeup_calibration
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_sensors_imu_wakeup_calibration
	CoreIMU
	CoreI2C
	CoreLED
)

target_link_custom_leka_targets(spike_lk_sensors_imu_wakeup_calibration)
