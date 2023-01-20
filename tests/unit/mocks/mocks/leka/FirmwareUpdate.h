// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/FirmwareUpdate.h"

namespace leka::mock {

class FirmwareUpdate : public interface::FirmwareUpdate
{
  public:
	MOCK_METHOD(Version, getCurrentVersion, (), (override));
	MOCK_METHOD(bool, isVersionAvailable, (const Version &), (override));
	MOCK_METHOD(bool, loadFirmware, (const Version &), (override));
};

}	// namespace leka::mock
