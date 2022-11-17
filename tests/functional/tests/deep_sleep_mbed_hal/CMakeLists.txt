# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_deep_sleep_mbed_hal

	INCLUDE_DIRECTORIES

	SOURCES
		suite_mbed_hal.cpp

	LINK_LIBRARIES
)
