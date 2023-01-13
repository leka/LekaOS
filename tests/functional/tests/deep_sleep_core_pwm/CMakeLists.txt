# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_deep_sleep_core_pwm

	INCLUDE_DIRECTORIES

	SOURCES
		suite_core_pwm.cpp

	LINK_LIBRARIES
		CorePwm
)
