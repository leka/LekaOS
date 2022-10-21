# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

set(TESTS_FUNCTIONAL_INCLUDE_DIR ${TESTS_FUNCTIONAL_DIR}/include)
set(TESTS_FUNCTIONAL_SOURCE_DIR ${TESTS_FUNCTIONAL_DIR}/source)
set(TESTS_FUNCTIONAL_TESTS_DIR ${TESTS_FUNCTIONAL_DIR}/tests)

add_subdirectory(${TESTS_FUNCTIONAL_TESTS_DIR}/boost_ut)
