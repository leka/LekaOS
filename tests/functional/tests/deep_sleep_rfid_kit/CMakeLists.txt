# Leka - LekaOS
# Copyright 2024 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_functional_test(
	TARGET
		functional_ut_deep_sleep_rfid_kit

	INCLUDE_DIRECTORIES

	SOURCES
		suite_rfid_kit.cpp

	LINK_LIBRARIES
		CoreBufferedSerial
		CoreRFIDReader
		RFIDKit
)
