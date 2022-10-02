# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(ColorKit STATIC)

include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare( glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.9.8
	GIT_PROGRESS TRUE
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(
	glm
)

target_include_directories(ColorKit
	PUBLIC
		include
		include/internal
)

target_sources(ColorKit
	PRIVATE
		source/ColorKit.cpp
		source/conversion.cpp
)

target_link_libraries(ColorKit
	glm::glm
)

if (${CMAKE_PROJECT_NAME} STREQUAL "LekaOSUnitTests")

	leka_unit_tests_sources(
		tests/ColorKit_test_conversion.cpp
		tests/ColorKit_test_gradient.cpp
		tests/RGB_test.cpp
	)

endif()
