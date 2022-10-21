# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(TESTS_FUNCTIONAL_SOURCES_DIR ${TESTS_FUNCTIONAL_DIR}/sources)

function(leka_functional_tests_includes TARGET_NAME UNIT_TESTS_INCLUDES)
	set(_UNIT_TESTS_INCLUDES ${UNIT_TESTS_INCLUDES} ${ARGN})
	target_include_directories(${TARGET_NAME}
		PRIVATE
		${TESTS_FUNCTIONAL_DIR}/include
		${_UNIT_TESTS_INCLUDES}
	)
endfunction()

function(leka_functional_tests_sources TARGET_NAME UNIT_TESTS_SOURCES)
	set(_UNIT_TESTS_SOURCES ${UNIT_TESTS_SOURCES} ${ARGN})
	target_sources(${TARGET_NAME}
		PRIVATE
		${TESTS_FUNCTIONAL_DIR}/main.cpp
		${_UNIT_TESTS_SOURCES}
	)
endfunction()

add_subdirectory(${TESTS_FUNCTIONAL_SOURCES_DIR}/IOExpander)
add_subdirectory(${TESTS_FUNCTIONAL_SOURCES_DIR}/QDAC)

add_custom_target(tests_functional)
add_dependencies(tests_functional
	suite_io_expander
	suite_qdac
)
