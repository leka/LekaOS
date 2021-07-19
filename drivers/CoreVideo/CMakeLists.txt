# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreVideo STATIC)

target_include_directories(CoreVideo
	PUBLIC
		include
		include/internal
		external/include
)

target_sources(CoreVideo
	PRIVATE
	source/CoreDMA2D.cpp
	source/CoreJPEG.cpp
	source/CoreLTDC.cpp
	source/CoreDSI.cpp
	source/CoreJPEG.cpp
	source/CoreJPEGMode.cpp
	source/CoreLCD.cpp
	source/CoreLTDC.cpp
	source/CoreLCDDriverOTM8009A.cpp
	source/CoreSDRAM.cpp
	external/source/st_jpeg_utils.cpp
)

target_link_libraries(CoreVideo
	PRIVATE
		mbed-os
		LKCoreLL
		LKCoreSTM32Hal
		LKCoreFatFs
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CGColor_test.cpp
		tests/CGPixel_test.cpp
		tests/CoreDMA2D_test.cpp
		tests/CoreDSI_test.cpp
		tests/CoreFont_test.cpp
		tests/CoreGraphics_test.cpp
		tests/CoreJPEG_test.cpp
		tests/CoreLCD_test.cpp
		tests/CoreLTDC_test.cpp
		tests/CoreLCDDriverOTM8009A_test.cpp
		tests/CoreSDRAM_test.cpp
		tests/CoreVideo_test.cpp
	)

endif()
