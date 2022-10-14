# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(suite_file_manager_kit)

target_include_directories(suite_file_manager_kit
	PRIVATE
	${TESTS_FUNCTIONAL_INCLUDE_DIR}
)

target_sources(suite_file_manager_kit
	PRIVATE
	${TESTS_FUNCTIONAL_DIR}/main.cpp
	./suite_file_manager_kit.cpp
)

target_link_libraries(suite_file_manager_kit
	FileManagerKit
)

target_link_custom_leka_targets(suite_file_manager_kit)
