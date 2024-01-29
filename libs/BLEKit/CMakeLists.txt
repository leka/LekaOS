# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_kit(BLEKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/BLEKit.cpp
		source/CoreGap.cpp
		source/CoreGapEventHandler.cpp
		source/CoreGattServer.cpp
		source/CoreGattServerEventHandler.cpp

	PUBLIC_LINK_LIBRARIES
		mbed-bluenrg-ms

	PRIVATE_LINK_LIBRARIES
		CoreEventQueue

	UNIT_TESTS_SOURCES
		tests/AdvertisingData_test.cpp
		tests/CoreGapEventHandler_test.cpp
		tests/CoreGap_test.cpp
		tests/CoreGattServerEventHandler_test.cpp
		tests/CoreGattServer_test.cpp
		tests/BLEKit_test.cpp
		tests/BLEServiceBattery_test.cpp
		tests/BLEServiceDeviceInformation_test.cpp
		tests/BLEServiceMonitoring_test.cpp
		tests/BLEServiceConfig_test.cpp
		tests/BLEServiceMagicCard_test.cpp
		tests/BLEServiceFileExchange_test.cpp
		tests/BLEServiceUpdate_test.cpp
)
