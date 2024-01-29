# Leka - LekaOS
# Copyright 2023 APF France handicap
# SPDX-License-Identifier: Apache-2.0

function(register_spike)
	set(options "")
	set(oneValueArgs TARGET)
	set(multiValueArgs INCLUDE_DIRECTORIES SOURCES LINK_LIBRARIES)

	cmake_parse_arguments(REGISTER_SPIKE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set(REGISTER_SPIKE_TARGET_FULL_NAME spike_${REGISTER_SPIKE_TARGET})

	add_executable(${REGISTER_SPIKE_TARGET_FULL_NAME})

	target_include_directories(${REGISTER_SPIKE_TARGET_FULL_NAME}
		PRIVATE
			${REGISTER_SPIKE_INCLUDE_DIRECTORIES}
	)

	target_sources(${REGISTER_SPIKE_TARGET_FULL_NAME}
		PRIVATE
			${REGISTER_SPIKE_SOURCES}
	)

	target_link_libraries(${REGISTER_SPIKE_TARGET_FULL_NAME}
		PRIVATE
			mbed-os
			${REGISTER_SPIKE_LINK_LIBRARIES}
	)

	target_compile_definitions(${REGISTER_SPIKE_TARGET_FULL_NAME}
		PUBLIC
			TARGET="${REGISTER_SPIKE_TARGET_FULL_NAME}"
	)

	mbed_set_post_build(${REGISTER_SPIKE_TARGET_FULL_NAME})

	add_dependencies(spikes
		${REGISTER_SPIKE_TARGET_FULL_NAME}
	)
endfunction(register_spike)
