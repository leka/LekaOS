# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_firmware_kit

	INCLUDE_DIRECTORIES

	SOURCES
		suite_firmware_kit.cpp

	LINK_LIBRARIES
		FirmwareKit
)
