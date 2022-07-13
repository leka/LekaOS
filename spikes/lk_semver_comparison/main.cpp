// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <string_view>

#include "rtos/ThisThread.h"

#include "LogKit.h"
#include "semver.h"			   // h2non
#include "semver/semver.hpp"   // Neargye

using namespace leka;
using namespace std::chrono_literals;

auto main() -> int
{
	logger::init();

	log_info("Hello, World!\n");

	// =========================================================================================== //

	auto version_numeral_major = uint8_t {1};
	auto version_numeral_minor = uint8_t {2};
	auto version_numeral_patch = uint16_t {3};

	auto version_string		   = std::string_view {"4.5.6"};
	const auto *version_carray = "7.8.9";

	// =========================================================================================== //
	rtos::ThisThread::sleep_for(1s);
	// =========================================================================================== //

	log_info("Semver by Neargye:");

	// Initialization
	auto version_numeral_neargye = semver::version {1, 2, 3};
	auto version_string_neargye	 = semver::version {version_string};
	auto version_carray_neargye	 = semver::version {version_carray};

	// Convert to string
	log_info("version_numeral_neargye is %s", version_numeral_neargye.to_string().c_str());
	log_info("version_string_neargye is %s", version_string_neargye.to_string().c_str());
	log_info("version_carray_neargye is %s", version_carray_neargye.to_string().c_str());

	// Compare 2 versions
	auto compare_versions_numeral_string_neargye = version_numeral_neargye < version_string_neargye;
	log_info("Numeral version is older than String version: %s",
			 compare_versions_numeral_string_neargye ? "True" : "False");

	// Range satisfaction
	auto range_string						= std::string_view {">=1.0.0 <2.0.0 || >5.4.3"};
	auto range_neargye						= semver::range::detail::range {range_string};
	auto is_version_string_in_range_neargye = range_neargye.satisfies(version_string_neargye, false);
	log_info("version string satisfy range %s: %s", range_string.data(),
			 is_version_string_in_range_neargye ? "True" : "False");
	auto is_version_carray_in_range_neargye = range_neargye.satisfies(version_carray_neargye, false);
	log_info("version carray satisfy range %s: %s", range_string.data(),
			 is_version_carray_in_range_neargye ? "True" : "False");

	// Destruction
	// do nothing

	/* other features:
		prerelease (alpha, beta, rc)
	*/

	// =========================================================================================== //
	log_info("\n\n\n");
	rtos::ThisThread::sleep_for(1s);
	// =========================================================================================== //

	log_info("Semver by h2non:");

	// Initialization
	auto version_numeral_h2non = semver_t {1, 2, 3};
	auto version_string_h2non  = semver_t {};
	semver_parse(version_string.data(), &version_string_h2non);
	auto version_carray_h2non = semver_t {};
	semver_parse(version_carray, &version_carray_h2non);

	// Convert to string
	std::array<char, 32> version_rendered_string {};
	semver_render(&version_numeral_h2non, version_rendered_string.data());
	log_info("version_numeral_h2non is %s", version_rendered_string.data());
	version_rendered_string.fill('\0');
	semver_render(&version_string_h2non, version_rendered_string.data());
	log_info("version_string_h2non is %s", version_rendered_string.data());
	version_rendered_string.fill('\0');
	semver_render(&version_carray_h2non, version_rendered_string.data());
	log_info("version_carray_h2non is %s", version_rendered_string.data());

	// Compare 2 versions
	auto compare_versions_numeral_string_h2non = semver_lt(version_numeral_h2non, version_string_h2non);
	log_info("Numeral version is older than String version: %s",
			 compare_versions_numeral_string_h2non == 1 ? "True" : "False");

	// Range satisfaction
	auto version_1_0_0 = semver_t {1, 0, 0};
	auto version_2_0_0 = semver_t {2, 0, 0};
	auto version_5_4_3 = semver_t {5, 4, 3};

	auto is_version_string_in_range_h2non =
		(semver_gte(version_string_h2non, version_1_0_0) && semver_lt(version_string_h2non, version_2_0_0)) ||
		semver_gt(version_string_h2non, version_5_4_3);
	log_info("version string satisfy range %s: %s", range_string.data(),
			 is_version_string_in_range_h2non ? "True" : "False");
	auto is_version_carray_in_range_h2non =
		(semver_gte(version_carray_h2non, version_1_0_0) && semver_lt(version_carray_h2non, version_2_0_0)) ||
		semver_gt(version_carray_h2non, version_5_4_3);
	log_info("version carray satisfy range %s: %s", range_string.data(),
			 is_version_carray_in_range_h2non ? "True" : "False");

	// Destruction
	semver_free(&version_numeral_h2non);
	semver_free(&version_string_h2non);
	semver_free(&version_carray_h2non);

	semver_free(&version_1_0_0);
	semver_free(&version_2_0_0);
	semver_free(&version_5_4_3);

	/* other features:
		Caret and Tilde operators
		Bump version
		Clean by removing invalid semver characters
	*/

	// =========================================================================================== //

	rtos::ThisThread::sleep_for(1s);

	return 0;
}
