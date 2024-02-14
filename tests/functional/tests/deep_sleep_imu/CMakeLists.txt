# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
	functional_ut_deep_sleep_imu

	INCLUDE_DIRECTORIES

	SOURCES
	suite_imu.cpp

	LINK_LIBRARIES
	CoreI2C
	CoreIMU
)
