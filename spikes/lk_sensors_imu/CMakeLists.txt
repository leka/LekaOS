# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(spike_lk_sensors_imu)

target_include_directories(spike_lk_sensors_imu
	PRIVATE
		.
)

target_sources(spike_lk_sensors_imu
	PRIVATE
		main.cpp
		LSM6DSOX_CommunicationI2C.cpp
		LSM6DSOX_ComponentAccelerometer.cpp
		LSM6DSOX_ComponentGyroscope.cpp
		LSM6DSOX_DataGatherer.cpp
		lsm6dsox_reg.c
)

target_link_libraries(spike_lk_sensors_imu
	CoreMotor
	CorePwm
)

target_link_custom_leka_targets(spike_lk_sensors_imu)
