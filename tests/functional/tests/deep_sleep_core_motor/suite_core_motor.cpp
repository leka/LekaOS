// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreMotor.h"
#include "CorePwm.h"
#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace leka;
using namespace boost::ut;
using namespace std::chrono;
using namespace boost::ut::bdd;

suite suite_core_motor = [] {
	scenario("base system check") = [] {
		given("motor is not instantiated") = [] {
			then("I expect deep sleep TO BE possible") = [] {
				auto status = utils::sleep::system_deep_sleep_check();

				expect(status.can_deep_sleep);
				expect(status.test_check_ok);
			};
		};
	};
};
