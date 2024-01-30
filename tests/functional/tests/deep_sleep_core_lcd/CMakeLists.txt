# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
	functional_ut_deep_sleep_core_lcd

	INCLUDE_DIRECTORIES

	SOURCES
	suite_core_lcd.cpp

	LINK_LIBRARIES
	CoreVideo
	CoreLL
	CoreSTM32Hal
	CorePwm
)
