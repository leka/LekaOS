// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Version.h"

namespace leka::interface {

class FirmwareUpdate
{
  public:
	virtual ~FirmwareUpdate() = default;

	virtual auto getCurrentVersion() -> Version				= 0;
	virtual auto loadUpdate(const Version &version) -> bool = 0;
};

}	// namespace leka::interface
