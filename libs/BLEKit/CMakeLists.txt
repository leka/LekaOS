# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(BLEKit STATIC)

target_include_directories(BLEKit
	PUBLIC
	include
)

target_sources(BLEKit
	PRIVATE
	source/BLEKit.cpp
	source/CoreGapEventHandler.cpp
	source/CoreGap.cpp
	source/CoreGattServerEventHandler.cpp
	source/CoreGattServer.cpp
)

target_link_libraries(BLEKit
	mbed-os
	CoreEventQueue
	RFIDKit
)

if(${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")
	leka_unit_tests_sources(
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
endif()
