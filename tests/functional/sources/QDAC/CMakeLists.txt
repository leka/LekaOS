# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_mbed_executable(suite_qdac)

target_include_directories(suite_qdac
	PRIVATE
	${TESTS_FUNCTIONAL_INCLUDE_DIR}
)

target_sources(suite_qdac
	PRIVATE
	${TESTS_FUNCTIONAL_DIR}/main.cpp
	./suite_qdac.cpp
)

target_link_libraries(suite_qdac
	CoreI2C
	CoreQDAC
)

target_link_custom_leka_targets(suite_qdac)
