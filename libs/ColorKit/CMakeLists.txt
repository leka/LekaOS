# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 0.9.9.8
	GIT_PROGRESS TRUE
	GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(
	glm
)

add_lib_kit(ColorKit
	PUBLIC_INCLUDE_DIRS
		include

	PRIVATE_SOURCES
		source/ColorKit.cpp
		source/conversion.cpp

	PRIVATE_LINK_LIBRARIES
		glm::glm

	UNIT_TESTS_SOURCES
		tests/ColorKit_test_conversion.cpp
		tests/ColorKit_test_gradient.cpp
		tests/RGB_test.cpp
)
