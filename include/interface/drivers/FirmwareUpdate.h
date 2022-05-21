// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "FirmwareVersion.h"

namespace leka::interface {

class FirmwareUpdate
{
  public:
	virtual ~FirmwareUpdate() = default;

	virtual auto loadUpdate(const FirmwareVersion &version) -> bool = 0;
};

}	// namespace leka::interface
