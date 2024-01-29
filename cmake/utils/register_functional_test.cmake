# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

function(register_functional_test)
	set(options "")
	set(oneValueArgs TARGET)
	set(multiValueArgs INCLUDE_DIRECTORIES SOURCES LINK_LIBRARIES)

	cmake_parse_arguments(REGISTER_FT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	add_executable(${REGISTER_FT_TARGET})

	target_include_directories(${REGISTER_FT_TARGET}
		PRIVATE
			${TESTS_FUNCTIONAL_INCLUDE_DIR}
			${REGISTER_FT_INCLUDE_DIRECTORIES}
	)

	target_sources(${REGISTER_FT_TARGET}
		PRIVATE
			${TESTS_FUNCTIONAL_SOURCE_DIR}/test_main.cpp
			${REGISTER_FT_SOURCES}
	)

	target_link_libraries(${REGISTER_FT_TARGET}
		mbed-os
		mbed-storage-blockdevice
		mbed-storage-sd
		mbed-storage-fat
		${REGISTER_FT_LINK_LIBRARIES}
	)

	target_compile_definitions(${REGISTER_FT_TARGET}
		PUBLIC
			TARGET="${REGISTER_FT_TARGET}"
	)

	mbed_set_post_build(${REGISTER_FT_TARGET})
endfunction()
