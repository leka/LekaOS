# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

register_spike(
	TARGET
		lk_update_process_app_base

	INCLUDE_DIRECTORIES
		.

	SOURCES
		main.cpp

	LINK_LIBRARIES
		mbed-storage-qspif
		bootutil
		CoreFlashMemory
		CoreQSPI
		FileManagerKit
		FirmwareKit
)
