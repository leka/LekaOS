// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include <cstddef>

#include "CoreFlashIS25LP016D.h"
#include "CoreFlashManagerIS25LP016D.h"
#include "CoreQSPI.h"
#include "FirmwareKit.h"
#include "os_version.h"
#include "semver/semver.hpp"
#include "tests/config.h"
#include "tests/utils.h"
#include "tests/utils_sleep.h"

using namespace leka;
using namespace boost::ut;
using namespace std::chrono;

auto qspi		   = CoreQSPI();
auto flash_manager = CoreFlashManagerIS25LP016D(qspi);
auto flash_memory  = CoreFlashIS25LP016D(qspi, flash_manager);

suite suite_firmware_kit = [] {
	"config version and firmware kit version are the same"_test = [&] {
		auto firmwarekit		 = FirmwareKit(flash_memory, FirmwareKit::DEFAULT_CONFIG);
		auto firmwarekit_version = firmwarekit.getCurrentVersion();

		auto semver_version = semver::version {OS_VERSION};

		expect(eq(firmwarekit_version.major, semver_version.major));
		expect(eq(firmwarekit_version.minor, semver_version.minor));
		expect(eq(firmwarekit_version.revision, semver_version.patch));
	};
};
