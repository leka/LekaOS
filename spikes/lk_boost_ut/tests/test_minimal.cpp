// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "./tests/config.h"
#include "boost/ut.hpp"

using namespace ut;

suite suite_minimal = [] { "minimal test"_test = [] { expect(1 != 2); }; };
