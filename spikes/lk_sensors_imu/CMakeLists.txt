# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_sensors_imu)

target_include_directories(spike_lk_sensors_imu
	PRIVATE
		.
		internal
		internal/include

)

target_sources(spike_lk_sensors_imu
	PRIVATE
		main.cpp
		internal/source/LSM6DSOX_CommunicationI2C.cpp
		internal/source/LSM6DSOX_ComponentAccelerometer.cpp
		internal/source/LSM6DSOX_ComponentGyroscope.cpp
		internal/source/LSM6DSOX_DataGatherer.cpp
		internal/source/lsm6dsox_reg.c
)

target_link_libraries(spike_lk_sensors_imu
	CoreMotor
	CorePwm
)

target_link_custom_leka_targets(spike_lk_sensors_imu)
