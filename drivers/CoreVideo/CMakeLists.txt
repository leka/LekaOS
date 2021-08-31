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
	source/LKCoreFont.cpp
	source/LKCoreGraphics.cpp
	source/LKCoreJPEG.cpp
	source/CoreLTDC.cpp
	source/CoreDSI.cpp
	source/LKCoreJPEG.cpp
	source/LKCoreLCD.cpp
	source/LKCoreLCDDriverOTM8009A.cpp
	source/CoreSDRAM.cpp
	source/LKCoreVideo.cpp
	external/source/st_jpeg_utils.cpp
)

target_link_libraries(CoreVideo
	PRIVATE
		mbed-os
		LKCoreLL
		LKCoreSTM32Hal
		CoreFs
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CGColor_test.cpp
		tests/CGPixel_test.cpp
		tests/CoreDMA2D_test.cpp
		tests/CoreDSI_test.cpp
		tests/LKCoreFont_test.cpp
		tests/LKCoreGraphics_test.cpp
		tests/LKCoreJPEG_test.cpp
		tests/LKCoreLCD_test.cpp
		tests/CoreLTDC_test.cpp
		tests/LKCoreLCDDriverOTM8009A_test.cpp
		tests/CoreSDRAM_test.cpp
		tests/LKCoreVideo_test.cpp
	)

endif()
