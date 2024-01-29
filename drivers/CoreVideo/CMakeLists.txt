# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_lib_core(CoreVideo
	PUBLIC_INCLUDE_DIRS
		include
		external/include

	PRIVATE_SOURCES
		source/CoreDMA2D.cpp
		source/CoreFont.cpp
		source/CoreGraphics.cpp
		source/JPEGImageProperties.cpp
		source/CoreJPEGModePolling.cpp
		source/CoreJPEGModeDMA.cpp
		source/CoreJPEG.cpp
		source/CoreLTDC.cpp
		source/CoreDSI.cpp
		source/CoreLCD.cpp
		source/CoreLCDDriverOTM8009A.cpp
		source/CoreSDRAM.cpp
		source/CoreVideo.cpp
		external/source/st_jpeg_utils.cpp

	PUBLIC_LINK_LIBRARIES
		CoreLL
	mbed_headers

	PRIVATE_LINK_LIBRARIES

	UNIT_TESTS_SOURCES
		external/source/HAL_IRQHandlers.cpp
		tests/CGColor_test.cpp
		tests/CGPixel_test.cpp
		tests/CoreDMA2D_test.cpp
		tests/CoreDSI_test.cpp
		tests/CoreFont_test.cpp
		tests/CoreGraphics_test.cpp
		tests/JPEGImageProperties_test.cpp
		tests/CoreJPEGModePolling_test.cpp
		tests/CoreJPEGModeDMA_test.cpp
		tests/CoreJPEG_test.cpp
		tests/CoreLCD_test.cpp
		tests/CoreLTDC_test.cpp
		tests/CoreLCDDriverOTM8009A_test.cpp
		tests/CoreSDRAM_test.cpp
		tests/CoreVideo_test.cpp
)
