# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_imu_app)

target_include_directories(spike_lk_imu_app
	PRIVATE
	.
)

target_sources(spike_lk_imu_app
	PRIVATE
	main.cpp
)

target_link_libraries(spike_lk_imu_app
	ColorKit
	CoreLED
	RFIDKit
	CoreIMU
)

target_link_custom_leka_targets(spike_lk_imu_app)
