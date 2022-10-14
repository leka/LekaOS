# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(TESTS_FUNCTIONAL_INCLUDE_DIR ${TESTS_FUNCTIONAL_DIR}/include)
set(TESTS_FUNCTIONAL_SOURCES_DIR ${TESTS_FUNCTIONAL_DIR}/sources)

add_subdirectory(${TESTS_FUNCTIONAL_SOURCES_DIR}/IOExpander)
add_subdirectory(${TESTS_FUNCTIONAL_SOURCES_DIR}/QDAC)

add_custom_target(tests_functional)
add_dependencies(tests_functional
	suite_io_expander
	suite_qdac
)
