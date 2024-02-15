# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_core_imu

	INCLUDE_DIRECTORIES
		.

	SOURCES
		suite_core_imu.cpp

	LINK_LIBRARIES
		CoreI2C
		CoreIMU
)
