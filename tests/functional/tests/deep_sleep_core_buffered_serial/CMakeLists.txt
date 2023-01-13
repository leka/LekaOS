# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_deep_sleep_core_buffered_serial

	INCLUDE_DIRECTORIES

	SOURCES
		suite_core_buffered_serial.cpp

	LINK_LIBRARIES
		CoreBufferedSerial
)
