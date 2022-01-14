# Leka - LekaOS
# Copyright 2020 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(CoreLED STATIC)

target_include_directories(CoreLED
	PUBLIC
		include

)

target_sources(CoreLED
	PRIVATE
		source/CoreLED.cpp
)

target_link_libraries(CoreLED
<<<<<<< HEAD
=======
	driver_FastLED
>>>>>>> feb9548f (:sparkles: Add CoreLED)
	mbed-os
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/CoreLED_test.cpp
	)

endif()
