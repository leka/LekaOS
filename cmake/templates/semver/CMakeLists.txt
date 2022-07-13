# Leka - LekaOS
# Copyright 2022 APF France handicap
# SPDX-License-Identifier: Apache-2.0

add_library(semver STATIC)

target_include_directories(semver
  PUBLIC
  .
)

target_sources(semver
  PRIVATE
  semver.c
)
