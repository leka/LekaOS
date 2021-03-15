# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(LKCoreVideo STATIC)

target_include_directories(LKCoreVideo
	PUBLIC
		include
		include/internal
		include/internal/jpeg
)

target_sources(LKCoreVideo
  PRIVATE
	source/LKCoreDMA2D.cpp
	source/LKCoreGraphics.cpp
	source/LKCoreJPEG.cpp
	source/LKCoreLTDC.cpp
	source/LKCoreDSI.cpp
	source/LKCoreJPEG.cpp
	source/LKCoreLTDC.cpp
	source/LKCoreLCDDriverOTM8009A.cpp
	source/LKCoreVideo.cpp
	source/st_jpeg_utils.cpp
	source/st_sdram.cpp
    source/LKCoreLCD.cpp
    source/LKCoreVideo.cpp
)

target_link_libraries(LKCoreVideo
	PRIVATE
		mbed-os
		LKCoreSTM32Hal
		LKCoreFatFs
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CGColor_test.cpp
		tests/LKCoreDMA2D_test.cpp
		tests/LKCoreDSI_test.cpp
		tests/LKCoreJPEG_test.cpp
		tests/LKCoreLCD_test.cpp
		tests/LKCoreLTDC_test.cpp
		tests/LKCoreLCDDriverOTM8009A_test.cpp
	)

endif()
