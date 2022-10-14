# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(suite_io_expander)

target_include_directories(suite_io_expander
	PRIVATE
	${TESTS_FUNCTIONAL_INCLUDE_DIR}
)

target_sources(suite_io_expander
	PRIVATE
	${TESTS_FUNCTIONAL_DIR}/main.cpp
	./suite_io_expander.cpp
)

target_link_libraries(suite_io_expander
	CoreI2C
	CoreIOExpander
)

target_link_custom_leka_targets(suite_io_expander)
