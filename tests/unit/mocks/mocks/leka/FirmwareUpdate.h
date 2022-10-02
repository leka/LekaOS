// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "interface/drivers/FirmwareUpdate.h"

namespace leka::mock {

class FirmwareUpdate : public interface::FirmwareUpdate
{
  public:
	MOCK_METHOD(bool, loadUpdate, (const FirmwareVersion &), (override));
};

}	// namespace leka::mock
